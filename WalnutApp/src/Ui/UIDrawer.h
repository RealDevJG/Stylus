#pragma once

#include "../CoreContext.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include <functional>
#include <memory>

namespace Stylus {

	class UIDrawer
	{
	public:
		UIDrawer(std::shared_ptr<CoreContext> context);

		void DrawToolButton(ToolEnum toolEnum, ToolData toolData, float width);
		void DrawToolOptions(std::function<void()> drawHook);
	private:
		std::shared_ptr<CoreContext> m_Context;
	};

}
