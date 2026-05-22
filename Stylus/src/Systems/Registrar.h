#pragma once

#include "ShaderRegistry.h"
#include "ToolRegistry.h"
#include "ToolSettingsStore.h"

namespace Stylus {

	class Registrar
	{
	public:
		void RegisterToolsAndShaders(ToolRegistry& toolRegistry, ShaderRegistry& shaderRegistry, ToolSettingsStore& settingsStore);
	};

}
