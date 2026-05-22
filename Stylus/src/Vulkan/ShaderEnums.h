#pragma once

#include <cstdint>

namespace Stylus {

	enum class ComputeShaderEnum : uint8_t
	{
		FillCanvasEffect = 0,
		BrushLikeTool
	};

	enum class GraphicsShaderEnum : uint8_t
	{
		BrushLikeToolOverlay = 0,
	};

}
