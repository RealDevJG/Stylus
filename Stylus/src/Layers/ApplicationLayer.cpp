#include "ApplicationLayer.h"

#include "../CoreContext.h"
#include "../Tools/ToolManager.h"
#include "../Vulkan/ShaderRegistry.h"

#include <Walnut/UI/UI.h>

namespace Stylus {

	void ApplicationLayer::OnAttach()
	{
		m_CoreContext = std::make_unique<CoreContext>();
		m_CoreContext->Init();

		ToolManager& toolManager = CoreContext::s_Instance->GetToolManager();
		toolManager.SetTool(ToolEnum::Brush);
	}

	void ApplicationLayer::OnDetach()
	{
		ShaderRegistry& shaderRegistry = CoreContext::s_Instance->GetShaderRegistry();
		shaderRegistry.Cleanup();
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
		ToolManager& toolManager = CoreContext::s_Instance->GetToolManager();
		ToolStore& toolStore = CoreContext::s_Instance->GetToolStore();

		ToolEnum toolEnum = toolStore.GetToolEnum(event.GetKeyCode());

		if (toolEnum == ToolEnum::None)
		{
			return false;
		}

		toolManager.SetTool(toolEnum);
		return true;
	}

}
