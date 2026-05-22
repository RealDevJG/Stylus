#pragma once

#include <Walnut/Image.h>

namespace Stylus {

	class ICanvasView
	{
	public:
		virtual ~ICanvasView() = default;
		[[nodiscard]] virtual const Walnut::Image& GetCanvasImage() const = 0;
	};

}
