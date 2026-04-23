#pragma once

#include "Tool.h"
#include "ToolEnum.h"

#include <unordered_map>

namespace Stylus {

	class ToolStore
	{
	public:
		ToolStore() = default;
		~ToolStore() = default;

		ToolStore(const ToolStore&) = delete;
		ToolStore& operator=(const ToolStore&) = delete;
		ToolStore(ToolStore&&) = delete;
		ToolStore& operator=(ToolStore&&) = delete;

		void Init();

		[[nodiscard]] std::shared_ptr<const Tool> GetTool(ToolEnum tool) const;
		[[nodiscard]] ToolEnum GetToolEnum(Walnut::KeyCode shortcut) const;

		[[nodiscard]] const std::unordered_map<ToolEnum, std::shared_ptr<const Tool>>& GetTools() const;
	private:
		std::unordered_map<ToolEnum, std::shared_ptr<const Tool>> m_Tools;
		std::unordered_map<Walnut::KeyCode, ToolEnum> m_KeyShortcuts;
	};

}
