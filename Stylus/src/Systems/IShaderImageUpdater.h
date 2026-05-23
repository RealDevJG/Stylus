#pragma once

#include <vulkan/vulkan.h>

namespace Stylus {

	class IShaderImageUpdater
	{
	public:
		virtual ~IShaderImageUpdater() = default;
		virtual void UpdateStorageImage(VkImageView imageView) = 0;
		virtual void UpdateFramebuffers(VkDevice device, VkImageView imageView, uint32_t width, uint32_t height) = 0;
	};

}
