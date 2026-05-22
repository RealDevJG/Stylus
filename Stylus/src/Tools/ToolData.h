#pragma once

#include "ToolEnum.h"
#include <Walnut/Input/KeyCodes.h>

#include <filesystem>
#include <string_view>

namespace Stylus {

	struct ToolData
	{
		std::string_view Name{};
		ToolEnum Type{};
		Walnut::KeyCode KeyShortcut{};

		std::filesystem::path ComputeShaderPath{};
		uint32_t ComputePushConstantSize{};

		std::filesystem::path VertShaderPath{};
		std::filesystem::path FragShaderPath{};
		uint32_t GraphicsPushConstantSize{};
	};

}
