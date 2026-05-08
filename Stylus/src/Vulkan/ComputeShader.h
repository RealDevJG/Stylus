#pragma once

#include <vulkan/vulkan.h>
#include <Walnut/Image.h>
#include <filesystem>

namespace Stylus {

	class ComputeShader
	{
	public:
		explicit ComputeShader(const std::filesystem::path& shaderPath, uint32_t pushSize);
		~ComputeShader();

		void SetImage(Walnut::Image* canvasImage);
		void DispatchShader(const void* pushData) const;
	private:
		void CreateComputePipeline(const std::filesystem::path& shaderPath);
		[[nodiscard]] VkShaderModule CreateShaderModule(const std::filesystem::path& shaderPath) const;

		void CreateLayouts();
		void CreatePools();
		void CreateCommandBuffer();

		void AllocateDescriptorSets();
		void UpdateDescriptorSets();
	private:
		VkDevice m_Device = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;

		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;

		VkPipeline m_ComputePipeline = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;

		uint32_t m_QueueFamilyIndex = (uint32_t)-1;
		Walnut::Image* m_CanvasImage;

		uint32_t m_PushSize = 0;
	};

}
