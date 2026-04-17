#pragma once

#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include <functional>

namespace Stylus {

	class UIDrawer
	{
	public:
		void DrawToolButton(ToolEnum toolEnum, ToolData toolData, float width) const;
		void DrawToolOptions(const std::function<void()> drawOptions) const;
	};

}
