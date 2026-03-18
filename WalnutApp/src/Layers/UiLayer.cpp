#include "UILayer.h"

#include <imgui.h>

void UiLayer::OnUIRender()
{
	ImGui::Begin("Tool Bar");
	ImGui::Text("Tool bar area");
	ImGui::End();

	ImGui::Begin("Tool Options");
	ImGui::Text("Tool options area");
	ImGui::End();
}
