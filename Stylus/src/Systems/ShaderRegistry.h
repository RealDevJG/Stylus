#pragma once

#include "../Vulkan/ComputeShader.h"
#include "../Vulkan/GraphicsShader.h"
#include "../Vulkan/ShaderEnums.h"

#include "IShaderImageUpdater.h"
#include "IShaderRegistryReadonly.h"

#include <memory>
#include <unordered_map>

namespace Stylus {

	class ShaderRegistry final : public IShaderImageUpdater, public IShaderRegistryReadonly
	{
	public:
		ShaderRegistry() = default;
		~ShaderRegistry();

		ShaderRegistry(const ShaderRegistry&) = delete;
		ShaderRegistry& operator=(const ShaderRegistry&) = delete;
		ShaderRegistry(ShaderRegistry&&) = delete;
		ShaderRegistry& operator=(ShaderRegistry&&) = delete;

		bool RegisterCompute(const ComputeShaderEnum shaderEnum, const std::filesystem::path& computePath, uint32_t pushSize);
		bool RegisterGraphics(const GraphicsShaderEnum shaderEnum, const std::filesystem::path& vertPath, const std::filesystem::path& fragPath, uint32_t pushSize);

		void UpdateStorageImage(VkImageView imageView) override;
		void UpdateFramebuffers(VkDevice device, VkImageView imageView, uint32_t width, uint32_t height) override;

		[[nodiscard]] ComputeShader* GetCompute(const ComputeShaderEnum shaderEnum) const override;
		[[nodiscard]] GraphicsShader* GetGraphics(const GraphicsShaderEnum shaderEnum) const override;
		[[nodiscard]] VkDescriptorSet GetDescriptorSet() const override;
		[[nodiscard]] VkFramebuffer GetFramebuffer() const override;
		[[nodiscard]] VkRenderPass GetRenderPass() const override;
	private:
		void Cleanup();
	private:
		std::unordered_map<ComputeShaderEnum, std::unique_ptr<ComputeShader>> m_ComputeShaders{};
		std::unordered_map<GraphicsShaderEnum, std::unique_ptr<GraphicsShader>> m_GraphicsShaders{};

		VkDescriptorSetLayout m_DescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool m_DescriptorPool{ VK_NULL_HANDLE };
		VkDescriptorSet m_DescriptorSet{ VK_NULL_HANDLE };
		VkFramebuffer m_Framebuffer{ VK_NULL_HANDLE };
		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };
	};

}
