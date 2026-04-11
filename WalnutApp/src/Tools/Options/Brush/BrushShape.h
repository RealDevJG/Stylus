#pragma once

#include <array>

namespace Stylus {

	enum class BrushShapeEnum
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
