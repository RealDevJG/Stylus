#include "ToolRegistry.h"

namespace Stylus {

	const Tool& ToolRegistry::GetTool(ToolEnum toolEnum) const
	{
		auto it = m_Tools.find(toolEnum);
		assert(it != m_Tools.end() && "Tool not found in ToolRegistry::GetCurrentTool");
		return *it->second;
	}

	ToolEnum ToolRegistry::GetToolEnum(Walnut::KeyCode shortcut) const
	{
		if (auto it = m_KeyShortcuts.find(shortcut); it != m_KeyShortcuts.end())
		{
			return it->second;
		}

		return ToolEnum::None;
	}

	const std::unordered_map<ToolEnum, std::unique_ptr<const Tool>>& ToolRegistry::GetTools() const
	{
		return m_Tools;
	}

}
