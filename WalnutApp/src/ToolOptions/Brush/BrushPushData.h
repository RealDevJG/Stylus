#pragma once

#include <glm/glm.hpp>

namespace Stylus {

	struct alignas(16) BrushPushData
	{
		glm::vec4 Colour;
		glm::vec2 MousePos;
		int Shape;
		float Radius;
		bool Antialiased;
	};

}
