#pragma once

#include <cstdint>

namespace Stylus {

	enum class ToolSettingsEnum : uint16_t
	{
		Width = 0,
		Shape,
		PrimaryColour,
		SecondaryColour,
		Antialiasing
	};

	using TSE = ToolSettingsEnum;

}
