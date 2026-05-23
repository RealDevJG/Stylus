#include "ComputeShader.h"

#include <iostream>

namespace Stylus {

	ComputeShader::~ComputeShader()
	{
		Destroy();
	}

	ComputeShader::ComputeShader(ComputeShader&& other) noexcept
		: m_Device(other.m_Device), m_PipelineLayout(other.m_PipelineLayout), m_Pipeline(other.m_Pipeline)
	{
		other.m_Device = VK_NULL_HANDLE;
		other.m_PipelineLayout = VK_NULL_HANDLE;
		other.m_Pipeline = VK_NULL_HANDLE;
	}

	ComputeShader& ComputeShader::operator=(ComputeShader&& other) noexcept
	{
		if (this != &other)
		{
			Destroy();

			m_Device = other.m_Device;
			m_PipelineLayout = other.m_PipelineLayout;
			m_Pipeline = other.m_Pipeline;

			other.m_Device = VK_NULL_HANDLE;
			other.m_PipelineLayout = VK_NULL_HANDLE;
			other.m_Pipeline = VK_NULL_HANDLE;
		}

		return *this;
	}

	bool ComputeShader::Init(VkDevice device, const std::filesystem::path& shaderPath, const ShaderLayoutConfig& layoutConfig)
	{
		if (layoutConfig.PushConstants.size() == 0)
		{
			return false;
		}

		m_Device = device;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(layoutConfig.DescriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = layoutConfig.DescriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = static_cast<uint32_t>(layoutConfig.PushConstants.size());
		pipelineLayoutInfo.pPushConstantRanges = layoutConfig.PushConstants.data();

		if (vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			std::cerr << "Failed to create pipeline layout in ComputeShader::Init\n";
			return false;
		}

		auto shaderCode = ReadShaderFile(shaderPath);

		if (shaderCode.size() == 0)
		{
			std::cerr << "Failed to read shader module in ComptueShader::Init";
			return false;
		}

		VkShaderModule shaderModule = CreateShaderModule(m_Device, shaderCode);

		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";

		VkComputePipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.stage = shaderStageInfo;
		pipelineInfo.layout = m_PipelineLayout;

		VkResult result = vkCreateComputePipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline);
		vkDestroyShaderModule(m_Device, shaderModule, nullptr);

		if (result != VK_SUCCESS)
		{
			std::cerr << "Failed to create compute pipeline in ComputeShader::Init\n";
			Destroy();
			return false;
		}

		return true;
	}

	void ComputeShader::Destroy()
	{
		if (m_Device != VK_NULL_HANDLE)
		{
			if (m_Pipeline != VK_NULL_HANDLE)
			{
				vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
				m_Pipeline = VK_NULL_HANDLE;
			}

			if (m_PipelineLayout != VK_NULL_HANDLE)
			{
				vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
				m_PipelineLayout = VK_NULL_HANDLE;
			}

			m_Device = VK_NULL_HANDLE;
		}
	}

	void ComputeShader::Bind(VkCommandBuffer commandBuffer) const
	{
		if (!IsValid())
		{
			return;
		}

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_Pipeline);
	}

	void ComputeShader::Dispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) const
	{
		if (!IsValid())
		{
			return;
		}

		vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
	}

	VkPipelineLayout ComputeShader::GetPipelineLayout() const
	{
		return m_PipelineLayout;
	}

	bool ComputeShader::IsValid() const
	{
		return m_Pipeline != VK_NULL_HANDLE;;
	}

}
