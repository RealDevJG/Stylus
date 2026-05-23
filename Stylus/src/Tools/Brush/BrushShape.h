#pragma once

#include <array>
#include <cstdint>

namespace Stylus {

	enum class BrushShapeEnum : uint8_t
	{
		Circle = 0,
		Square,
		Triangle
	};

	constexpr auto BrushShapeList = std::to_array<std::pair<BrushShapeEnum, const char*>>({
		{ BrushShapeEnum::Circle, "Circle" },
		{ BrushShapeEnum::Square, "Square" },
		{ BrushShapeEnum::Triangle, "Triangle" }
	});

}
