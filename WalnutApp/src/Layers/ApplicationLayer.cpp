#include "ApplicationLayer.h"

#include <Walnut/UI/UI.h>

void ApplicationLayer::OnUIRender()
{

}

void ApplicationLayer::UI_DrawAboutModal()
{
	if (!m_AboutModalOpen)
		return;

	ImGui::OpenPopup("About");
	m_AboutModalOpen = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (m_AboutModalOpen)
	{
		auto image = Walnut::Application::Get().GetApplicationIcon();
		ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

		ImGui::SameLine();
		Walnut::UI::ShiftCursorX(20.0f);

		ImGui::BeginGroup();
		ImGui::Text("Walnut application framework");
		ImGui::Text("by Studio Cherno.");
		ImGui::EndGroup();

		if (Walnut::UI::ButtonCentered("Close"))
		{
			m_AboutModalOpen = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ApplicationLayer::ShowAboutModal()
{
	m_AboutModalOpen = true;
}
