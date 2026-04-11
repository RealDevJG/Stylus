#pragma once

#include <filesystem>
#include <string>

namespace Stylus {

	struct ToolData
	{
		std::string Name;
		std::filesystem::path ShaderPath;
		size_t PushConstantStructSize;
		// TODO: may need a left click shader path too
		// TODO: tool options here and repurpose ToolOptionsRegistry as repository or resolver?
	};

}
