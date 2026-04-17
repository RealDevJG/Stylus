#pragma once

#include "Tools/ToolManager.h"
#include "Tools/ToolOptionsRegistry.h"
#include "Tools/ToolStore.h"
#include "Vulkan/ShaderRegistry.h"

#include <memory>

namespace Stylus {

	class CoreContext
	{
	public:
		inline static CoreContext* s_Instance = nullptr;

		CoreContext();
		void Init();

		ToolManager& GetToolManager();
		ToolOptionsRegistry& GetToolOptionsRegistry();
		ToolStore& GetToolStore();
		ShaderRegistry& GetShaderRegistry();
	private:
		std::unique_ptr<ToolManager> m_ToolManager;
		std::unique_ptr<ToolOptionsRegistry> m_OptionsRegistry;
		std::unique_ptr<ShaderRegistry> m_ShaderRegistry;
		std::unique_ptr<ToolStore> m_ToolStore;
	};

}
