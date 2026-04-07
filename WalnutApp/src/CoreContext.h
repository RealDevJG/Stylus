#pragma once

#include "Tools/ToolManager.h"
#include "ToolOptions/ToolOptionsRegistry.h"
#include "Vulkan/ShaderRegistry.h"

#include <memory>

namespace Stylus {

	struct CoreContext
	{
		std::shared_ptr<ToolOptionsRegistry> OptionsRegistry;
		std::shared_ptr<ShaderRegistry> ShaderRegistry;
		std::shared_ptr<ToolManager> ToolManager;
	};

}
