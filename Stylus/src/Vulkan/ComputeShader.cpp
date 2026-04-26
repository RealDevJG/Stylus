#include "ComputeShader.h"

#include "../Serialisation/FileReader.h"
#include <Walnut/Application.h>

#include <cassert>
#include <iostream>

namespace Stylus {

	ComputeShader::ComputeShader(const std::filesystem::path& shaderPath, uint32_t pushSize)
		: m_Device(Walnut::Application::GetDevice()),
  		  m_PhysicalDevice(Walnut::Application::GetPhysicalDevice()),
  		  m_QueueFamilyIndex(Walnut::Application::GetQueueFamilyIndex()),
		  m_PushSize(pushSize)
	{
		CreateLayouts();
		CreateComputePipeline(shaderPath);
		CreatePools();
		CreateCommandBuffer();

		AllocateDescriptorSets();
	}

	ComputeShader::~ComputeShader()
	{
		// Wait for GPU to finish before deleting
		vkDeviceWaitIdle(m_Device);

		vkDestroyPipeline(m_Device, m_ComputePipeline, nullptr);
		vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);

		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);

		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
	}

	void ComputeShader::SetImage(std::shared_ptr<Walnut::Image> canvasImage)
	{
		vkDeviceWaitIdle(m_Device);

		m_CanvasImage = canvasImage;
		UpdateDescriptorSets();
	}

	void ComputeShader::DispatchShader(const void* pushData)
	{
		assert(m_CanvasImage && "[ComputeShader] DispatchShader was called without using SetImage() first\n");

		if (!m_CanvasImage)
		{
			std::cerr << "[ComputeShader] DispatchShader was called without using SetImage() first\n";
			return;
		}

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		vkResetCommandBuffer(m_CommandBuffer, 0);
		vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);

		// Transition Image to General (write)
		VkImageMemoryBarrier barrierToCompute{};
		barrierToCompute.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrierToCompute.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrierToCompute.newLayout = VK_IMAGE_LAYOUT_GENERAL;
		barrierToCompute.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		barrierToCompute.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		barrierToCompute.image = m_CanvasImage->GetImage();
		barrierToCompute.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrierToCompute.subresourceRange.levelCount = 1;
		barrierToCompute.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(
			m_CommandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Wait for ImGui to finish drawing
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, // Before compute starts
			0, 0, nullptr, 0, nullptr, 1, &barrierToCompute
		);

		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_ComputePipeline);
		vkCmdBindDescriptorSets(m_CommandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);

		vkCmdPushConstants(
			m_CommandBuffer,
			m_PipelineLayout,
			VK_SHADER_STAGE_COMPUTE_BIT,
			0,
			m_PushSize,
			pushData
		);

		uint32_t groupCountX = static_cast<uint32_t>(ceil(m_CanvasImage->GetWidth() / 16.0f));
		uint32_t groupCountY = static_cast<uint32_t>(ceil(m_CanvasImage->GetHeight() / 16.0f));
		vkCmdDispatch(m_CommandBuffer, groupCountX, groupCountY, 1);

		// Transition Image from to ReadOnly for ImGui
		VkImageMemoryBarrier barrierToGraphics = barrierToCompute;
		barrierToGraphics.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
		barrierToGraphics.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrierToGraphics.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		barrierToGraphics.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
			m_CommandBuffer,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, // Wait for compute to finish
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Before ImGui reads it again
			0, 0, nullptr, 0, nullptr, 1, &barrierToGraphics
		);

		vkEndCommandBuffer(m_CommandBuffer);

		// Submitting to queue
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		VkQueue computeQueue{};
		vkGetDeviceQueue(m_Device, m_QueueFamilyIndex, 0, &computeQueue);

		// Submit and wait for it to finish (for simplicity rn, not optimal for performance)
		// TODO: go for performance route
		vkQueueSubmit(computeQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(computeQueue);
	}

	void ComputeShader::CreateComputePipeline(const std::filesystem::path& shaderPath)
	{
		VkShaderModule shaderModule = CreateShaderModule(shaderPath);

		VkPipelineShaderStageCreateInfo shaderStage{};
		shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderStage.module = shaderModule;
		shaderStage.pName = "main";

		VkComputePipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.stage = shaderStage;
		pipelineInfo.layout = m_PipelineLayout;

		VkResult result = vkCreateComputePipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_ComputePipeline);
		check_vk_result(result);

		vkDestroyShaderModule(m_Device, shaderModule, nullptr);
	}

	VkShaderModule ComputeShader::CreateShaderModule(const std::filesystem::path& shaderPath)
	{
		Stylus::FileReader fileReader(shaderPath);
		std::vector<uint32_t> fileBuffer = fileReader.Read();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.codeSize = fileBuffer.size() * sizeof(uint32_t);
		createInfo.pCode = fileBuffer.data();

		VkShaderModule shaderModule{};
		VkResult result = vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule);
		check_vk_result(result);

		return shaderModule;
	}

	void ComputeShader::CreateLayouts()
	{
		VkDescriptorSetLayoutBinding imageBinding{};
		imageBinding.binding = 0;
		imageBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		imageBinding.descriptorCount = 1;
		imageBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		imageBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &imageBinding;

		VkResult result = vkCreateDescriptorSetLayout(m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout);
		check_vk_result(result);

		// Push Constants
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = m_PushSize;

		// Pipeline layout
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		result = vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);
		check_vk_result(result);
	}

	void ComputeShader::CreatePools()
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		poolSize.descriptorCount = 1;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = 1;

		vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool);
	}

	void ComputeShader::CreateCommandBuffer()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = m_QueueFamilyIndex;

		vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		vkAllocateCommandBuffers(m_Device, &allocInfo, &m_CommandBuffer);
	}

	void ComputeShader::AllocateDescriptorSets()
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &m_DescriptorSetLayout;

		vkAllocateDescriptorSets(m_Device, &allocInfo, &m_DescriptorSet);
	}

	void ComputeShader::UpdateDescriptorSets()
	{
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageView = m_CanvasImage->GetImageView();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_DescriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(m_Device, 1, &descriptorWrite, 0, nullptr);
	}

}
