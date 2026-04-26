#include "UIDrawer.h"

#include "../Systems/ToolManager.h"
#include <imgui.h>

namespace Stylus {

	void UIDrawer::DrawToolButton(ToolEnum toolEnum, std::string_view toolName) const
	{
		const float width = ImGui::GetContentRegionAvail().x;

		if (ImGui::Button(toolName.data(), ImVec2(width, 35)))
		{
			m_ToolManager->SetTool(toolEnum);
		}
	}

	void UIDrawer::DrawToolOptions(const std::function<void()> drawOptions) const
	{
		drawOptions();
	}

}
