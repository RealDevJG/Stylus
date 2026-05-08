#pragma once

#include "../Tools/Tool.h"
#include "../Tools/ToolEnum.h"

#include <unordered_map>
#include <memory>

namespace Stylus {

	class ToolSettingsRegistry;
	class ShaderRegistry;

	class ToolRegistry
	{
	public:
		ToolRegistry(std::shared_ptr<ToolSettingsRegistry> toolSettingsRegistry, std::shared_ptr<ShaderRegistry> shaderRegistry);
		~ToolRegistry() = default;

		ToolRegistry(const ToolRegistry&) = delete;
		ToolRegistry& operator=(const ToolRegistry&) = delete;
		ToolRegistry(ToolRegistry&&) = delete;
		ToolRegistry& operator=(ToolRegistry&&) = delete;

		[[nodiscard]] const Tool* GetTool(ToolEnum tool) const;
		[[nodiscard]] ToolEnum GetToolEnum(Walnut::KeyCode shortcut) const;

		[[nodiscard]] const std::unordered_map<ToolEnum, std::unique_ptr<const Tool>>& GetTools() const;
	private:
		void RegisterTools(ToolSettingsRegistry* toolSettingsRegistry, ShaderRegistry* shaderRegistry);
	private:
		std::unordered_map<ToolEnum, std::unique_ptr<const Tool>> m_Tools;
		std::unordered_map<Walnut::KeyCode, ToolEnum> m_KeyShortcuts;
	};

}
