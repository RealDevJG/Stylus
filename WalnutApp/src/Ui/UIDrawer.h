#pragma once

#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include <functional>

namespace Stylus {

	class UIDrawer
	{
	public:
		void DrawToolButton(ToolEnum toolEnum, ToolData toolData) const;
		void DrawToolOptions(const std::function<void()> drawOptions) const;
	};

}
