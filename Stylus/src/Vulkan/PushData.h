#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <cstdint>

namespace Stylus {

	struct alignas(16) BrushLikePushData
	{
		glm::vec4 Colour;
		glm::vec2 MousePos;
		glm::vec2 PrevMousePos;
		int Shape;
		float Width;
		uint32_t Antialiased;
	};

	struct alignas(16) FillCanvasPushData
	{
		glm::vec4 Colour;
	};

}
