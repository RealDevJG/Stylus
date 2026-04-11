#include "UILayer.h"

#include "../Tools/Options/Brush/BrushShape.h"

#include <imgui.h>
#include <iostream>
#include <string>

UiLayer::UiLayer(std::shared_ptr<Stylus::CoreContext> context)
	: m_Context(context) {}

void UiLayer::OnUIRender()
{
	Stylus::ToolEnum currentTool = m_Context->ToolManager->GetTool();

	ImGui::Begin("Tool Bar");
	float width = ImGui::GetContentRegionAvail().x;

	auto tools = m_Context->OptionsRegistry->GetTools();

	for (auto tool : tools)
	{
		std::string str = std::to_string(static_cast<int>(tool));
		const char* t = str.c_str();

		if (ImGui::Button(t, ImVec2(width, 35)))
		{
			std::cout << "button " << static_cast<int>(tool) << "\n";
		}
	}

	ImGui::End();

	ImGui::Begin("Tool Options");

	auto hooks = m_Context->OptionsRegistry->GetHooks(currentTool);
	hooks.DrawUI();

	ImGui::End();
}
