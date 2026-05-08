#include "ApplicationLayer.h"

#include "../Layers/CanvasLayer.h"
#include "../Layers/UiLayer.h"

#include "../Systems/ToolManager.h"
#include "../Systems/ToolRegistry.h"
#include "../Systems/ToolSettingsController.h"

#include <Walnut/Application.h>

namespace Stylus {

	ApplicationLayer::ApplicationLayer(
		std::shared_ptr<ToolManager> toolManager,
		std::shared_ptr<ToolRegistry> toolRegistry,
		std::shared_ptr<ToolSettingsController> toolSettingsController
	) : m_ToolManager(toolManager), m_ToolRegistry(toolRegistry), m_ToolSettingsController(toolSettingsController) {}

	void ApplicationLayer::OnAttach()
	{
		SetMenubarCallback();
	}

	void ApplicationLayer::OnEvent(Walnut::Event& event)
	{
		Walnut::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Walnut::KeyPressedEvent>([this](Walnut::KeyPressedEvent& e) { return OnKeyPressed(e); });
	}

	bool ApplicationLayer::OnKeyPressed(Walnut::KeyPressedEvent& event)
	{
		Walnut::KeyCode keyCode = event.GetKeyCode();

		if (keyCode == Walnut::KeyCode::LeftBracket || keyCode == Walnut::KeyCode::RightBracket)
		{
			float moveBy = static_cast<float>(keyCode) - 92.0f;
			m_ToolSettingsController->ChangeBrushWidthBy(moveBy);
			return true;
		}

		if (auto toolEnum = m_ToolRegistry->GetToolEnum(keyCode); toolEnum != ToolEnum::None)
		{
			m_ToolManager->SetTool(toolEnum);
			return true;
		}

		return false;
	}

	void ApplicationLayer::SetMenubarCallback() const
	{
		Walnut::Application& app = Walnut::Application::Get();

		app.SetMenubarCallback(
			[&app]()
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit"))
					{
						app.Close();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo (ctrl+z)"))
					{
						CanvasLayer* canvasLayer = app.GetLayer<CanvasLayer>();
						canvasLayer->UndoHistory();
					}

					if (ImGui::MenuItem("Redo (ctrl+y)"))
					{
						CanvasLayer* canvasLayer = app.GetLayer<CanvasLayer>();
						canvasLayer->RedoHistory();
					}

					if (ImGui::MenuItem("Resize Canvas"))
					{
						UiLayer* uiLayer = app.GetLayer<UiLayer>();
						uiLayer->OpenResizeCanvasModal();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Default Window Layout"))
					{
						UiLayer* uiLayer = app.GetLayer<UiLayer>();
						uiLayer->SetDefaultLayout();
					}

					ImGui::EndMenu();
				}
			}
		);
	}

}
