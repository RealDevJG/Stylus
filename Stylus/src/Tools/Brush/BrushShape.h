#pragma once

#include <array>
#include <cstdint>

namespace Stylus {

	enum class BrushShapeEnum : uint8_t
	{
		Circle = 0,
		Square,
		Triangle,
		FlippedTriangle,
		Pentagon,
		Diamond,
		Hexagon,
		Octagon,
		Hexagram,
		Pentagram,
		Heart
	};

	constexpr auto BrushShapeList = std::to_array<std::pair<BrushShapeEnum, const char*>>({
		{ BrushShapeEnum::Circle, "Circle" },
		{ BrushShapeEnum::Square, "Square" },
		{ BrushShapeEnum::Triangle, "Triangle" },
		{ BrushShapeEnum::FlippedTriangle, "Flipped Triangle" },
		{ BrushShapeEnum::Pentagon, "Pentagon" },
		{ BrushShapeEnum::Diamond, "Diamond" },
		{ BrushShapeEnum::Hexagon, "Hexagon" },
		{ BrushShapeEnum::Octagon, "Octagon" },
		{ BrushShapeEnum::Hexagram, "Hexagram" },
		{ BrushShapeEnum::Pentagram, "Pentagram" },
		{ BrushShapeEnum::Heart, "Heart" },
	});

}
