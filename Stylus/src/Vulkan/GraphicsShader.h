#pragma once

#include "ShaderConfig.h"

#include <vulkan/vulkan.h>
#include <filesystem>

namespace Stylus {

	class GraphicsShader
	{
	public:
		GraphicsShader() = default;
		~GraphicsShader();

		GraphicsShader(const GraphicsShader&) = delete;
		GraphicsShader& operator=(const GraphicsShader&) = delete;
		GraphicsShader(GraphicsShader&& other) noexcept;
		GraphicsShader& operator=(GraphicsShader&& other) noexcept;

		[[nodiscard]] bool Init(VkDevice device, VkRenderPass renderPass, const std::filesystem::path& vertPath, const std::filesystem::path& fragPath, const ShaderLayoutConfig& layoutConfig);
		void Destroy();

		void Bind(VkCommandBuffer commandBuffer) const;
		void Draw(VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0) const;

		[[nodiscard]] VkPipelineLayout GetPipelineLayout() const;
		[[nodiscard]] bool IsValid() const;
	private:
		VkDevice m_Device{ VK_NULL_HANDLE };
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_Pipeline{ VK_NULL_HANDLE };
	};

}
