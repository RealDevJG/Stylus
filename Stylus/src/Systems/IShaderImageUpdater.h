#pragma once

#include <vulkan/vulkan.h>

namespace Stylus {

	class IShaderImageUpdater
	{
	public:
		virtual ~IShaderImageUpdater() = default;
		virtual void UpdateStorageImage(VkImageView imageView) = 0;
	};

}
