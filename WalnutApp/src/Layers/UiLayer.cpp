#include "UILayer.h"

#include "../ToolOptions/Brush/BrushShape.h"

#include <imgui.h>

static char g_BrushWidthString[100] = "13";

static Stylus::BrushShapeEnum g_CurrentShape = Stylus::BrushShapeList[1].first;
static std::string g_ShapeLabel(Stylus::BrushShapeList[1].second);

UiLayer::UiLayer(std::shared_ptr<Stylus::CoreContext> context)
	: m_Context(context)
{}

void UiLayer::OnUIRender()
{
	ImGui::Begin("Tool Bar");
	ImGui::Text("Tool bar area");
	ImGui::End();

	ImGui::Begin("Tool Options");
	//ImGui::Text("Tool options area");

	ImGui::InputText("Brush Width", g_BrushWidthString, IM_ARRAYSIZE(g_BrushWidthString));
	
	if (ImGui::BeginCombo("Brush Shape", g_ShapeLabel.c_str()))
	{
		for (const auto& [shapeEnum, shapeString] : Stylus::BrushShapeList)
		{
			bool isSelected = g_CurrentShape == shapeEnum;

			if (ImGui::Selectable(shapeString, isSelected))
			{
				g_CurrentShape = shapeEnum;
				g_ShapeLabel = shapeString;
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}
