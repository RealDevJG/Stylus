#pragma once

#include <glm/glm.hpp>

namespace Stylus {

	struct alignas(16) BrushPushData
	{
		glm::vec4 Colour;
		glm::vec2 MousePos;
		glm::vec2 PrevMousePos;
		int Shape;
		float Radius;
		uint32_t Antialiased;
	};

}
