#pragma once

#include "ShaderConfig.h"

#include <vulkan/vulkan.h>
#include <filesystem>

namespace Stylus {

	class ComputeShader
	{
	public:
		ComputeShader() = default;
		~ComputeShader();

		ComputeShader(const ComputeShader&) = delete;
		ComputeShader& operator=(const ComputeShader&) = delete;
		ComputeShader(ComputeShader&& other) noexcept;
		ComputeShader& operator=(ComputeShader&& other) noexcept;

		[[nodiscard]] bool Init(VkDevice device, const std::filesystem::path& shaderPath, const ShaderLayoutConfig& layoutConfig);
		void Destroy();

		void Bind(VkCommandBuffer commandBuffer) const;
		void Dispatch(VkCommandBuffer commandBuffer, uint32_t groupCountX = 16, uint32_t groupCountY = 16, uint32_t groupCountZ = 1) const;

		[[nodiscard]] VkPipelineLayout GetPipelineLayout() const;
		[[nodiscard]] bool IsValid() const;
	private:
		VkDevice m_Device{ VK_NULL_HANDLE };
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_Pipeline{ VK_NULL_HANDLE };
	};

}
