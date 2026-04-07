#include "UILayer.h"

#include "../Tools/Options/Brush/BrushShape.h"

#include <imgui.h>

UiLayer::UiLayer(std::shared_ptr<Stylus::CoreContext> context)
	: m_Context(context) {}

void UiLayer::OnUIRender()
{
	ImGui::Begin("Tool Bar");
	ImGui::Text("Tool bar area");
	ImGui::End();

	ImGui::Begin("Tool Options");

	Stylus::ToolEnum currentTool = m_Context->ToolManager->GetTool();
	auto hooks = m_Context->OptionsRegistry->GetHooks(currentTool);

	hooks.DrawUI();

	ImGui::End();
}
