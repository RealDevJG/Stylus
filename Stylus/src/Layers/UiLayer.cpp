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
		std::weak_ptr<const Tool> currentTool = m_ToolManager->GetTool();

		ImGui::Begin("Tool Bar");

		const auto& tools = m_ToolRegistry->GetTools();
		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			m_UiDrawer.DrawToolButton(toolEnum, toolData);
		}

		ImGui::End();

		ImGui::Begin("Tool Options");

		// TODO: make m_UiDrawer do the work so it can be extended in the future
		if (auto tool = currentTool.lock())
		{
			tool->DrawOptionsUI();
		}

		ImGui::End();
	}

}
