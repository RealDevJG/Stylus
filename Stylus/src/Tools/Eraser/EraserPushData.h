#pragma once

#include <glm/glm.hpp>

namespace Stylus {

	struct alignas(16) EraserPushData
	{
		EraserPushData(
			glm::vec2 mousePos,
			glm::vec2 prevMousePos,
			int shape,
			float width,
			uint8_t antialiased
		) : Colour(0.0f), MousePos(mousePos), PrevMousePos(prevMousePos), Shape(shape), Width(width), Antialiased(antialiased)
		{}

		const glm::vec4 Colour;
		glm::vec2 MousePos;
		glm::vec2 PrevMousePos;
		int Shape;
		float Width;
		uint8_t Antialiased;
	};

}
