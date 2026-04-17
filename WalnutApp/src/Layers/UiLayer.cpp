#include "UILayer.h"

#include "../CoreContext.h"
#include "../Tools/Tool.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"
#include "../Tools/ToolManager.h"
#include "../Tools/ToolOptionsRegistry.h"
#include "../Tools/ToolStore.h"

#include <imgui.h>

namespace Stylus {

	void UiLayer::OnUIRender()
	{
		ToolManager& toolManager = CoreContext::s_Instance->GetToolManager();
		std::weak_ptr<const Tool> currentTool = toolManager.GetTool();

		ToolStore& toolStore = CoreContext::s_Instance->GetToolStore();
		ToolOptionsRegistry& optionsRegistry = CoreContext::s_Instance->GetToolOptionsRegistry();

		ImGui::Begin("Tool Bar");

		float width = ImGui::GetContentRegionAvail().x;
		const auto& tools = toolStore.GetTools();

		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			m_UiDrawer.DrawToolButton(toolEnum, toolData, width);
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
