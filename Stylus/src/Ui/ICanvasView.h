#pragma once

#include <Walnut/Image.h>
#include <vulkan/vulkan.h>

namespace Stylus {

	class ICanvasView
	{
	public:
		virtual ~ICanvasView() = default;
		[[nodiscard]] virtual const Walnut::Image& GetCanvasImage() const = 0;
		[[nodiscard]] virtual const Walnut::Image& GetOverlayImage() const = 0;
	};

}
