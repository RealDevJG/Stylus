#include "ApplicationLayer.h"

#include "../Tools/ToolManager.h"
#include "../Tools/ToolRegistry.h"
#include "../Vulkan/ShaderRegistry.h"

#include <Walnut/UI/UI.h>

namespace Stylus {

	void ApplicationLayer::OnAttach()
	{
		m_ToolManager->SetTool(ToolEnum::Brush);
	}

	void ApplicationLayer::OnDetach()
	{
		m_ShaderRegistry->Cleanup();
	}

	void ApplicationLayer::OnEvent(Walnut::Event& event)
	{
		Walnut::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Walnut::KeyPressedEvent>([this](Walnut::KeyPressedEvent& e) { return OnKeyPressed(e); });
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

	bool ApplicationLayer::OnKeyPressed(Walnut::KeyPressedEvent& event)
	{
		ToolEnum toolEnum = m_ToolRegistry->GetToolEnum(event.GetKeyCode());

		if (toolEnum == ToolEnum::None)
		{
			return false;
		}

		m_ToolManager->SetTool(toolEnum);
		return true;
	}

}
