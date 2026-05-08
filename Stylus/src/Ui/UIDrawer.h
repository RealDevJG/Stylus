#pragma once

#include "../Tools/ToolEnum.h"

#include <functional>
#include <string_view>

namespace Stylus {

	class ToolManager;

	class UIDrawer
	{
	public:
		UIDrawer(ToolManager* toolManager)
			: m_ToolManager(toolManager) {}

		void DrawToolButton(ToolEnum toolEnum, std::string_view toolName) const;
		void DrawToolOptions(const std::function<void()> drawOptions) const;
	private:
		ToolManager* m_ToolManager;
	};

}
