#include "UIDrawer.h"

#include <imgui.h>

namespace Stylus {

	UIDrawer::UIDrawer(std::shared_ptr<CoreContext> context)
	: m_Context(context) {}

	void Stylus::UIDrawer::DrawToolButton(ToolEnum toolEnum, ToolData toolData, float width)
	{
		if (ImGui::Button(toolData.Name.c_str(), ImVec2(width, 35)))
		{
			m_Context->ToolManager->SetTool(toolEnum);
		}
	}

	void Stylus::UIDrawer::DrawToolOptions(std::function<void()> drawHook)
	{
		drawHook();
	}

}
