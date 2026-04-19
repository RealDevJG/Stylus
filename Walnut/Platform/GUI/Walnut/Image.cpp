#include "Image.h"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"

#include "ApplicationGUI.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Walnut {

	namespace Utils {

		static uint32_t GetVulkanMemoryType(VkMemoryPropertyFlags properties, uint32_t type_bits)
		{
			VkPhysicalDeviceMemoryProperties prop;
			vkGetPhysicalDeviceMemoryProperties(Application::GetPhysicalDevice(), &prop);
			for (uint32_t i = 0; i < prop.memoryTypeCount; i++)
			{
				if ((prop.memoryTypes[i].propertyFlags & properties) == properties && type_bits & (1 << i))
					return i;
			}
			
			return 0xffffffff;
		}

		static uint32_t BytesPerPixel(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGBA:    return 4;
				case ImageFormat::RGBA32F: return 16;
			}
			return 0;
		}
		
		static VkFormat WalnutFormatToVulkanFormat(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::RGBA:    return VK_FORMAT_R8G8B8A8_UNORM;
				case ImageFormat::RGBA32F: return VK_FORMAT_R32G32B32A32_SFLOAT;
			}
			return (VkFormat)0;
		}

	}

	Image::Image(std::string_view path)
		: m_Filepath(path)
	{
		int width, height, channels;
		uint8_t* data = nullptr;

		if (stbi_is_hdr(m_Filepath.c_str()))
		{
			data = (uint8_t*)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 4);
			m_Format = ImageFormat::RGBA32F;
		}
		else
		{
			data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 4);
			m_Format = ImageFormat::RGBA;
		}

		m_Width = width;
		m_Height = height;
		
		AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
		SetData(data);
		stbi_image_free(data);
	}

	Image::Image(uint32_t width, uint32_t height, ImageFormat format, const void* data)
		: m_Width(width), m_Height(height), m_Format(format)
	{
		AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
		if (data)
			SetData(data);
	}

	Image::~Image()
	{
		Release();
	}

	void Image::AllocateMemory(uint64_t size)
	{
		VkDevice device = Application::GetDevice();

		VkResult err;
		
		VkFormat vulkanFormat = Utils::WalnutFormatToVulkanFormat(m_Format);

		// Create the Image
		{
			VkImageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;
			info.format = vulkanFormat;
			info.extent.width = m_Width;
			info.extent.height = m_Height;
			info.extent.depth = 1;
			info.mipLevels = 1;
			info.arrayLayers = 1;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			// NOTE: Added VK_IMAGE_USAGE_STORAGE_BIT and VK_IMAGE_USAGE_TRANSFER_SRC_BIT as a part of Stylus
			info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			err = vkCreateImage(device, &info, nullptr, &m_Image);
			check_vk_result(err);
			VkMemoryRequirements req;
			vkGetImageMemoryRequirements(device, m_Image, &req);
			VkMemoryAllocateInfo alloc_info = {};
			alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			alloc_info.allocationSize = req.size;
			alloc_info.memoryTypeIndex = Utils::GetVulkanMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, req.memoryTypeBits);
			err = vkAllocateMemory(device, &alloc_info, nullptr, &m_Memory);
			check_vk_result(err);
			err = vkBindImageMemory(device, m_Image, m_Memory, 0);
			check_vk_result(err);
		}

		// Create the Image View:
		{
			VkImageViewCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = m_Image;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = vulkanFormat;
			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.layerCount = 1;
			err = vkCreateImageView(device, &info, nullptr, &m_ImageView);
			check_vk_result(err);
		}

		// Create sampler:
		{
			VkSamplerCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.magFilter = VK_FILTER_LINEAR;
			info.minFilter = VK_FILTER_LINEAR;
			info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.minLod = -1000;
			info.maxLod = 1000;
			info.maxAnisotropy = 1.0f;
			VkResult err = vkCreateSampler(device, &info, nullptr, &m_Sampler);
			check_vk_result(err);
		}

		// Create the Descriptor Set:
		m_DescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_Sampler, m_ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		// NOTE: added by stylus
		m_ReadbackBufferSize = m_Width * m_Height * Utils::BytesPerPixel(m_Format);

		VkBufferCreateInfo buffer_info = {};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size = m_ReadbackBufferSize;
		buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkCreateBuffer(device, &buffer_info, nullptr, &m_ReadbackBuffer);

		VkMemoryRequirements req;
		vkGetBufferMemoryRequirements(device, m_ReadbackBuffer, &req);

		VkMemoryAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = req.size;
		alloc_info.memoryTypeIndex = Utils::GetVulkanMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, req.memoryTypeBits);

		vkAllocateMemory(device, &alloc_info, nullptr, &m_ReadbackBufferMemory);
		vkBindBufferMemory(device, m_ReadbackBuffer, m_ReadbackBufferMemory, 0);
	}

	void Image::Release()
	{
		Application::SubmitResourceFree([sampler = m_Sampler, imageView = m_ImageView, image = m_Image,
			memory = m_Memory, stagingBuffer = m_StagingBuffer, stagingBufferMemory = m_StagingBufferMemory,
			// NOTE: added by stylus
			readbackBuffer = m_ReadbackBuffer, readbackMemory = m_ReadbackBufferMemory]()
		{
			VkDevice device = Application::GetDevice();

			vkDestroySampler(device, sampler, nullptr);
			vkDestroyImageView(device, imageView, nullptr);
			vkDestroyImage(device, image, nullptr);
			vkFreeMemory(device, memory, nullptr);
			vkDestroyBuffer(device, stagingBuffer, nullptr);
			vkFreeMemory(device, stagingBufferMemory, nullptr);
	
			// NOTE: added by stylus
			vkDestroyBuffer(device, readbackBuffer, nullptr);
			vkFreeMemory(device, readbackMemory, nullptr);
		});

		m_Sampler = nullptr;
		m_ImageView = nullptr;
		m_Image = nullptr;
		m_Memory = nullptr;
		m_StagingBuffer = nullptr;
		m_StagingBufferMemory = nullptr;

		// NOTE: added by stylus
		m_ReadbackBuffer = nullptr;
		m_ReadbackBufferMemory = nullptr;
	}

	void Image::SetData(const void* data)
	{
		VkDevice device = Application::GetDevice();

		size_t upload_size = m_Width * m_Height * Utils::BytesPerPixel(m_Format);

		VkResult err;

		if (!m_StagingBuffer)
		{
			// Create the Upload Buffer
			{
				VkBufferCreateInfo buffer_info = {};
				buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				buffer_info.size = upload_size;
				buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				err = vkCreateBuffer(device, &buffer_info, nullptr, &m_StagingBuffer);
				check_vk_result(err);
				VkMemoryRequirements req;
				vkGetBufferMemoryRequirements(device, m_StagingBuffer, &req);
				m_AlignedSize = req.size;
				VkMemoryAllocateInfo alloc_info = {};
				alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				alloc_info.allocationSize = req.size;
				alloc_info.memoryTypeIndex = Utils::GetVulkanMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, req.memoryTypeBits);
				err = vkAllocateMemory(device, &alloc_info, nullptr, &m_StagingBufferMemory);
				check_vk_result(err);
				err = vkBindBufferMemory(device, m_StagingBuffer, m_StagingBufferMemory, 0);
				check_vk_result(err);
			}

		}

		// Upload to Buffer
		{
			char* map = NULL;
			err = vkMapMemory(device, m_StagingBufferMemory, 0, m_AlignedSize, 0, (void**)(&map));
			check_vk_result(err);
			memcpy(map, data, upload_size);
			VkMappedMemoryRange range[1] = {};
			range[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
			range[0].memory = m_StagingBufferMemory;
			range[0].size = m_AlignedSize;
			err = vkFlushMappedMemoryRanges(device, 1, range);
			check_vk_result(err);
			vkUnmapMemory(device, m_StagingBufferMemory);
		}


		// Copy to Image
		{
			VkCommandBuffer command_buffer = Application::GetCommandBuffer(true);

			VkImageMemoryBarrier copy_barrier = {};
			copy_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			copy_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			copy_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			copy_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			copy_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			copy_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			copy_barrier.image = m_Image;
			copy_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copy_barrier.subresourceRange.levelCount = 1;
			copy_barrier.subresourceRange.layerCount = 1;
			vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &copy_barrier);

			VkBufferImageCopy region = {};
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.layerCount = 1;
			region.imageExtent.width = m_Width;
			region.imageExtent.height = m_Height;
			region.imageExtent.depth = 1;
			vkCmdCopyBufferToImage(command_buffer, m_StagingBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

			VkImageMemoryBarrier use_barrier = {};
			use_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			use_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			use_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			use_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			use_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			use_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			use_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			use_barrier.image = m_Image;
			use_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			use_barrier.subresourceRange.levelCount = 1;
			use_barrier.subresourceRange.layerCount = 1;
			vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &use_barrier);

			Application::FlushCommandBuffer(command_buffer);
		}
	}

	std::vector<uint8_t> Image::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		// Safety check
		if (x + width > m_Width || y + height > m_Height)
			return {};

		VkDevice device = Application::GetDevice();
		VkCommandBuffer commandBuffer = Application::GetCommandBuffer(true);

		VkImageMemoryBarrier copyBarrier = {};
		copyBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		copyBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
		copyBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		copyBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		copyBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		copyBarrier.image = m_Image;
		copyBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyBarrier.subresourceRange.levelCount = 1;
		copyBarrier.subresourceRange.layerCount = 1;
		copyBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
		copyBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &copyBarrier
		);

		VkBufferImageCopy region = {};
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { (int32_t)x, (int32_t)y, 0 };
		region.imageExtent = { width, height, 1 };

		vkCmdCopyImageToBuffer(commandBuffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_ReadbackBuffer, 1, &region);

		VkImageMemoryBarrier revertBarrier = copyBarrier;
		revertBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		revertBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
		revertBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		revertBarrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &revertBarrier
		);

		Application::FlushCommandBuffer(commandBuffer);

		uint32_t bytesToRead = width * height * Utils::BytesPerPixel(m_Format);
		std::vector<uint8_t> resultData(bytesToRead);

		void* mappedData;
		vkMapMemory(device, m_ReadbackBufferMemory, 0, bytesToRead, 0, &mappedData);
		memcpy(resultData.data(), mappedData, bytesToRead);
		vkUnmapMemory(device, m_ReadbackBufferMemory);

		return resultData;
	}

	void Image::Resize(uint32_t width, uint32_t height)
	{
		if (m_Image && m_Width == width && m_Height == height)
			return;

		// TODO: max size?

		m_Width = width;
		m_Height = height;

		Release();
		AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
	}

	void* Image::Decode(const void* buffer, uint64_t length, uint32_t& outWidth, uint32_t& outHeight)
	{
		int width, height, channels;
		uint8_t* data = nullptr;
		uint64_t size = 0;

		data = stbi_load_from_memory((const stbi_uc*)buffer, length, &width, &height, &channels, 4);
		size = width * height * 4;

		outWidth = width;
		outHeight = height;

		return data;
	}

}
