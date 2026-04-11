#include "UILayer.h"

#include "../Tools/Options/Brush/BrushShape.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include <imgui.h>
#include <unordered_map>

UiLayer::UiLayer(std::shared_ptr<Stylus::CoreContext> context)
	: m_Context(context), m_UiDrawer(context) {}

void UiLayer::OnUIRender()
{
	Stylus::ToolEnum currentTool = m_Context->ToolManager->GetTool();

	ImGui::Begin("Tool Bar");

	float width = ImGui::GetContentRegionAvail().x;
	const std::unordered_map<Stylus::ToolEnum, Stylus::ToolData>& tools = m_Context->ToolStore->GetTools();

	for (const auto& [toolEnum, toolData] : tools)
	{
		m_UiDrawer.DrawToolButton(toolEnum, toolData, width);
	}

	ImGui::End();

	ImGui::Begin("Tool Options");

	auto hooks = m_Context->OptionsRegistry->GetHooks(currentTool);
	m_UiDrawer.DrawToolOptions(hooks.DrawUI);

	ImGui::End();
}
