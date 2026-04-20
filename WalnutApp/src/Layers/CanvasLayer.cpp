#include "CanvasLayer.h"

#include "../CoreContext.h"
#include "../Tools/ToolManager.h"
#include "../Tools/Fill/FillCanvasData.h"
#include "../Vulkan/ShaderRegistry.h"

namespace Stylus {

	static uint32_t g_ComputeQueueFamily = (uint32_t)-1;

	CanvasLayer::CanvasLayer(uint32_t canvasWidth, uint32_t canvasHeight)
		: m_CanvasWidth(canvasWidth), m_CanvasHeight(canvasHeight) {}

	void CanvasLayer::OnAttach()
	{
		m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);

		ShaderRegistry& shaderRegistry = CoreContext::s_Instance->GetShaderRegistry();
		shaderRegistry.SetCanvasImage(m_CanvasImage);

		auto fillCanvasShader = shaderRegistry.Get(EffectEnum::FillCanvas);

		FillCanvasPushData pushData{ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };
		fillCanvasShader->DispatchShader(&pushData);
	}

	void CanvasLayer::OnUIRender()
	{
		ImGui::Begin("Canvas");

		const uint32_t width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		const uint32_t height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		if (!m_CanvasImage || m_CanvasImage->GetWidth() != width || m_CanvasImage->GetHeight() != height)
		{
			m_CanvasWidth = width;
			m_CanvasHeight = height;

			OnAttach();
		}

		ImGui::Image(m_CanvasImage->GetDescriptorSet(), { static_cast<float>(m_CanvasWidth), static_cast<float>(m_CanvasHeight) });
		m_IsCanvasHovered = ImGui::IsItemHovered();

		ImVec2 minImageBounds = ImGui::GetItemRectMin();
		ImVec2 maxImageBounds = ImGui::GetItemRectMax();
		ImVec2 imGuiMousePos = ImGui::GetMousePos();

		float x = imGuiMousePos.x - minImageBounds.x;
		float y = imGuiMousePos.y - minImageBounds.y;

		m_MousePos = glm::vec2(x, y);

		ImGui::End();
	}

	void CanvasLayer::OnUpdate(float ts)
	{
		ToolManager& toolManager = CoreContext::s_Instance->GetToolManager();

		if (ImGui::IsMouseClicked(0) && m_IsCanvasHovered)
		{
			m_LeftMouseDown = true;
		}
		else if (ImGui::IsMouseClicked(1) && m_IsCanvasHovered)
		{
			m_RightMouseDown = true;
		}

		if (ImGui::IsMouseReleased(0))
		{
			m_LeftMouseDown = false;
		}

		if (ImGui::IsMouseReleased(1))
		{
			m_RightMouseDown = false;
		}

		if (m_LeftMouseDown)
		{
			toolManager.UseLeftClick(m_MousePos, m_PrevMousePos);
		}
		else if (m_RightMouseDown)
		{
			toolManager.UseRightClick(m_MousePos, m_PrevMousePos);
		}

		m_PrevMousePos = m_MousePos;
	}

}
