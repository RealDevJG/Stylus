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
			float width,
			uint8_t antialiased
		) : Colour(colour), MousePos(mousePos), PrevMousePos(prevMousePos), Shape(shape), Width(width), Antialiased(antialiased)
		{}

		glm::vec4 Colour;
		glm::vec2 MousePos;
		glm::vec2 PrevMousePos;
		int Shape;
		float Width;
		uint32_t Antialiased;
	};

}
