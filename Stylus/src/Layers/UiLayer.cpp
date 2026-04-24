#include "UILayer.h"

#include "../Tools/Tool.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"
#include "../Tools/ToolManager.h"
#include "../Tools/ToolRegistry.h"

#include <imgui.h>

namespace Stylus {

	void UiLayer::OnUIRender()
	{
		ImGui::Begin("Tool Bar");

		const auto& tools = m_ToolRegistry->GetTools();
		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			m_UiDrawer.DrawToolButton(toolEnum, toolData.Name);
		}

		ImGui::End();

		ImGui::Begin("Tool Options");

		std::weak_ptr<const Tool> currentTool = m_ToolManager->GetTool();
		if (auto tool = currentTool.lock())
		{
			m_UiDrawer.DrawToolOptions([&tool]() { tool->DrawOptionsUI(); });
		}

		ImGui::End();
	}

}
