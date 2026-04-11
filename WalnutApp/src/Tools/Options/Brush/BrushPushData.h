#pragma once

#include <glm/glm.hpp>

namespace Stylus {

	struct alignas(16) BrushPushData
	{
		BrushPushData(
			glm::vec4 colour,
			glm::vec2 mousePos,
			glm::vec2 prevMousePos,
			int shape,
			float radius,
			uint8_t antialiased
		) : Colour(colour), MousePos(mousePos), PrevMousePos(prevMousePos), Shape(shape), Radius(radius), Antialiased(antialiased)
		{}

		glm::vec4 Colour;
		glm::vec2 MousePos;
		glm::vec2 PrevMousePos;
		int Shape;
		float Radius;
		uint32_t Antialiased;
	};

}
