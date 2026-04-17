#pragma once

#include <filesystem>
#include <string>

namespace Stylus {

	struct ToolData
	{
		std::string Name;
		std::filesystem::path ShaderPath;
		size_t PushConstantStructSize;
	};

}
