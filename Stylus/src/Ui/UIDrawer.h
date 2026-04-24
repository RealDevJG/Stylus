#pragma once

#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include <functional>
#include <memory>

namespace Stylus {

	class ToolManager;

	class UIDrawer
	{
	public:
		UIDrawer(std::shared_ptr<ToolManager> toolManager)
			: m_ToolManager(toolManager) {}

		void DrawToolButton(ToolEnum toolEnum, ToolData toolData) const;
		void DrawToolOptions(const std::function<void()> drawOptions) const;
	private:
		std::shared_ptr<ToolManager> m_ToolManager;
	};

}
