#include "ToolActionExecutor.h"

#include "../Vulkan/ComputeShader.h"
#include "../Vulkan/GraphicsShader.h"

#include <glm/glm.hpp>
#include <Walnut/Application.h>

namespace Stylus {

	ToolActionExecutor::ToolActionExecutor(const IShaderRegistryReadonly& shaderRegistryReadonly, const ICanvasView& canvasView, ToolSettingsStore& settingsStore)
		: m_ShaderRegistryReadonly(shaderRegistryReadonly), m_ICanvasView(canvasView), m_SettingsStore(settingsStore) {}

	bool ToolActionExecutor::Execute(ToolAction& action) const
	{
		switch (action.Type)
		{
		case ToolActionType::ComputeShaderAction:
			return ExecuteComputeShader(std::get<ComputeShaderAction>(action.Data));
		case ToolActionType::GraphicsShaderAction:
			return ExecuteGraphicsShader(std::get<GraphicsShaderAction>(action.Data));
		case ToolActionType::UpdateSettingValueAction:
			return UpdateSettingValue(std::get<UpdateSettingValueAction>(action.Data));
		}

		return false;
	}

	bool ToolActionExecutor::ExecuteComputeShader(const ComputeShaderAction& computeAction) const
	{
		ComputeShader* shader = m_ShaderRegistryReadonly.GetCompute(computeAction.ShaderEnum);

		if (shader == nullptr || !shader->IsValid())
		{
			std::cerr << "Shader is invalid in ToolActionExecutor::ExecuteComputeShader\n";
			return false;
		}

		VkCommandBuffer cmd = Walnut::Application::GetCommandBuffer(true);
		shader->Bind(cmd);

		const Walnut::Image& canvasImage = m_ICanvasView.GetCanvasImage();
		VkDescriptorSet canvasDescriptorSet = m_ShaderRegistryReadonly.GetDescriptorSet();

		VkPipelineLayout pipelineLayout = shader->GetPipelineLayout();
		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &canvasDescriptorSet, 0, nullptr);
		vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, computeAction.PushDataSize, computeAction.PushData.data());

		uint32_t groupX = static_cast<uint32_t>(glm::ceil(canvasImage.GetWidth() / 16.0f));
		uint32_t groupY = static_cast<uint32_t>(glm::ceil(canvasImage.GetHeight() / 16.0f));
		shader->Dispatch(cmd, groupX, groupY, 1);

		Walnut::Application::FlushCommandBuffer(cmd);

		return true;
	}

	bool ToolActionExecutor::ExecuteGraphicsShader(const GraphicsShaderAction& graphicsAction) const
	{
		GraphicsShader* shader = m_ShaderRegistryReadonly.GetGraphics(graphicsAction.ShaderEnum);

		if (shader == nullptr || !shader->IsValid())
		{
			std::cerr << "Shader is invalid in ToolActionExecutor::ExecuteGraphicsShader\n";
			return false;
		}

		VkCommandBuffer cmd = Walnut::Application::GetCommandBuffer(true);

		const Walnut::Image& canvasImage = m_ICanvasView.GetCanvasImage();
		uint32_t width = canvasImage.GetWidth();
		uint32_t height = canvasImage.GetHeight();

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_ShaderRegistryReadonly.GetRenderPass();

		renderPassInfo.framebuffer = m_ShaderRegistryReadonly.GetFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { width, height };

		vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		shader->Bind(cmd);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(width);
		viewport.height = static_cast<float>(height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cmd, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { width, height };
		vkCmdSetScissor(cmd, 0, 1, &scissor);

		VkDescriptorSet canvasDescriptorSet = m_ShaderRegistryReadonly.GetDescriptorSet();
		VkPipelineLayout pipelineLayout = shader->GetPipelineLayout();

		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &canvasDescriptorSet, 0, nullptr);
		vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, graphicsAction.PushDataSize, graphicsAction.PushData.data());

		constexpr uint32_t vertexCount = 3;
		shader->Draw(cmd, vertexCount);

		vkCmdEndRenderPass(cmd);
		Walnut::Application::FlushCommandBuffer(cmd);

		return false;
	}

	bool ToolActionExecutor::UpdateSettingValue(const UpdateSettingValueAction& updateSettingAction) const
	{
		updateSettingAction.SettingsUpdater->Apply(m_SettingsStore);
		return false;
	}

}
