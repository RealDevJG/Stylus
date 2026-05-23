#pragma once

#include "../Tools/Tool.h"
#include "../Tools/ToolEnum.h"
#include "IToolRegistryReadonly.h"

#include <Walnut/Input/KeyCodes.h>
#include <unordered_map>
#include <memory>

namespace Stylus {

	class ToolRegistry final : public IToolRegistryReadonly
	{
	public:
		ToolRegistry() = default;
		~ToolRegistry() = default;

		ToolRegistry(const ToolRegistry&) = delete;
		ToolRegistry& operator=(const ToolRegistry&) = delete;
		ToolRegistry(ToolRegistry&&) = delete;
		ToolRegistry& operator=(ToolRegistry&&) = delete;

		template<typename T> requires std::derived_from<T, Tool>
		void Register(ToolEnum toolEnum, const T& tool)
		{
			const ToolData& toolData = tool.GetToolData();

			m_KeyShortcuts.try_emplace(toolData.KeyShortcut, toolData.Type);
			m_Tools.try_emplace(toolData.Type, std::make_unique<T>(tool));
		}

		[[nodiscard]] const Tool& GetTool(ToolEnum toolEnum) const override;
		[[nodiscard]] ToolEnum GetToolEnum(Walnut::KeyCode shortcut) const override;
		[[nodiscard]] const std::unordered_map<ToolEnum, std::unique_ptr<const Tool>>& GetTools() const override;
	private:
		std::unordered_map<ToolEnum, std::unique_ptr<const Tool>> m_Tools;
		std::unordered_map<Walnut::KeyCode, ToolEnum> m_KeyShortcuts;
	};

}
