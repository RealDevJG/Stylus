#include "UIDrawer.h"

#include "../Tools/ToolManager.h"
#include <imgui.h>

namespace Stylus {

	void UIDrawer::DrawToolButton(ToolEnum toolEnum, ToolData toolData) const
	{
		const float width = ImGui::GetContentRegionAvail().x;

		if (ImGui::Button(toolData.Name.c_str(), ImVec2(width, 35)))
		{
			m_ToolManager->SetTool(toolEnum);
		}
	}

	// NOTE: no longer used, to be remade in the future
	void UIDrawer::DrawToolOptions(const std::function<void()> drawOptions) const
	{
		drawOptions();
	}

}
