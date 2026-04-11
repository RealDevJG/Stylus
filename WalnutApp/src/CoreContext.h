#pragma once

#include "Tools/ToolManager.h"
#include "Tools/ToolStore.h"
#include "Tools/Options/ToolOptionsRegistry.h"

#include "Vulkan/ShaderRegistry.h"

#include <memory>

namespace Stylus {

	struct CoreContext
	{
		std::shared_ptr<ToolOptionsRegistry> OptionsRegistry;
		std::shared_ptr<ShaderRegistry> ShaderRegistry;
		std::shared_ptr<ToolStore> ToolStore;
		std::shared_ptr<ToolManager> ToolManager;
	};

}
