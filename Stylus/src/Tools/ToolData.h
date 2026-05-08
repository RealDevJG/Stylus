#pragma once

#include <Walnut/Input/KeyCodes.h>

#include <filesystem>
#include <string>

namespace Stylus {

	struct ToolData
	{
		std::string Name;
		std::filesystem::path ShaderPath{};
		uint32_t PushConstantStructSize{};
		Walnut::KeyCode KeyShortcut = Walnut::KeyCode::None;
	};

}
