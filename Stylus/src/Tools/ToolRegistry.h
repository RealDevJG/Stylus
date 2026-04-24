#pragma once

#include "Tool.h"
#include "ToolEnum.h"

#include <unordered_map>
#include <memory>

namespace Stylus {

	class ToolSettingsRegistry;
	class ShaderRegistry;

	class ToolRegistry
	{
	public:
		ToolRegistry() = default;
		~ToolRegistry() = default;

		ToolRegistry(const ToolRegistry&) = delete;
		ToolRegistry& operator=(const ToolRegistry&) = delete;
		ToolRegistry(ToolRegistry&&) = delete;
		ToolRegistry& operator=(ToolRegistry&&) = delete;

		void Init(std::shared_ptr<ToolSettingsRegistry> toolSettingsRegistry, std::shared_ptr<ShaderRegistry> shaderRegistry);

		[[nodiscard]] std::shared_ptr<const Tool> GetTool(ToolEnum tool) const;
		[[nodiscard]] ToolEnum GetToolEnum(Walnut::KeyCode shortcut) const;

		[[nodiscard]] const std::unordered_map<ToolEnum, std::shared_ptr<const Tool>>& GetTools() const;
	private:
		std::unordered_map<ToolEnum, std::shared_ptr<const Tool>> m_Tools;
		std::unordered_map<Walnut::KeyCode, ToolEnum> m_KeyShortcuts;
	};

}
