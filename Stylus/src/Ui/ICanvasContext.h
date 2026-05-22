#pragma once

#include <cstdint>

namespace Stylus {

	class ICanvasContext
	{
	public:
		virtual ~ICanvasContext() = default;
		virtual void CreateCanvas(uint32_t width, uint32_t height) = 0;
		[[nodiscard]] virtual float GetCanvasScale() const = 0;
	};

}
