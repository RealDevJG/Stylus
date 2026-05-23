#include "EditorLayer.h"

#include "../Systems/Registrar.h"
#include <Walnut/Application.h>

namespace Stylus {

	void EditorLayer::OnAttach()
	{
		InitSystems();
		SetMenubarCallback();
	}

	void EditorLayer::OnUpdate(float ts)
	{
		m_TimeStep = ts;

		if (m_CanvasCoordinator->IsCanvasViewportHovered() && !m_SpaceDown)
		{
			glm::vec2 prevCanvasMousePos = m_CanvasMousePos;
			m_CanvasMousePos = m_CanvasCoordinator->GetCanvasMousePos();
			m_ShouldAddToHistory = m_ToolManager->TryUseClickAction(m_LeftMouseDown, m_RightMouseDown, m_CanvasMousePos, prevCanvasMousePos);
		}
	}

	void EditorLayer::OnUIRender()
	{
		m_CanvasCoordinator->Render();
		m_UIManager->Render();
	}

	void EditorLayer::OnEvent(Walnut::Event& event)
	{
		Walnut::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Walnut::KeyPressedEvent>([this](Walnut::KeyPressedEvent& e) { return OnKeyPressed(e); });
		dispatcher.Dispatch<Walnut::KeyReleasedEvent>([this](Walnut::KeyReleasedEvent& e) { return OnKeyReleased(e); });
		dispatcher.Dispatch<Walnut::MousePressedEvent>([this](Walnut::MousePressedEvent& e) { return OnMousePressed(e); });
		dispatcher.Dispatch<Walnut::MouseReleasedEvent>([this](Walnut::MouseReleasedEvent& e) { return OnMouseReleased(e); });
		dispatcher.Dispatch<Walnut::MouseScrolledEvent>([this](Walnut::MouseScrolledEvent& e) { return OnMouseScrolled(e); });
		dispatcher.Dispatch<Walnut::MouseMovedEvent>([this](Walnut::MouseMovedEvent& e) { return OnMouseMoved(e); });
	}

	void EditorLayer::InitSystems()
	{
		m_ShaderRegistry = std::make_unique<ShaderRegistry>();
		m_ToolRegistry = std::make_unique<ToolRegistry>();
		m_ToolSettingsStore = std::make_unique<ToolSettingsStore>();
		m_ToolSettingsController = std::make_unique<ToolSettingsController>(*m_ToolSettingsStore);
		m_HistoryManager = std::make_unique<HistoryManager<std::vector<uint8_t>>>();
		m_Canvas = std::make_unique<Canvas>();
		m_CanvasViewport = std::make_unique<CanvasViewport>();
		m_ActionExecutor = std::make_unique<ToolActionExecutor>(*m_ShaderRegistry, *m_Canvas, *m_ToolSettingsStore);
		m_CanvasCoordinator = std::make_unique<CanvasCoordinator>(*m_Canvas, *m_CanvasViewport, *m_HistoryManager, *m_ActionExecutor, *m_ShaderRegistry);
		m_ToolManager = std::make_unique<ToolManager>(*m_ActionExecutor, *m_ToolRegistry);
		m_UIManager = std::make_unique<UIManager>(*m_ToolManager, *m_ToolRegistry, *m_CanvasCoordinator, *m_ActionExecutor);

		Registrar registrar{};
		registrar.RegisterToolsAndShaders(*m_ToolRegistry, *m_ShaderRegistry, *m_ToolSettingsStore);

		m_ToolManager->SetTool(ToolEnum::Brush);
		m_CanvasCoordinator->CreateCanvas(854, 480);
		m_UIManager->SetDefaultLayout();
	}

	void EditorLayer::SetMenubarCallback() const
	{
		Walnut::Application& app = Walnut::Application::Get();

		app.SetMenubarCallback(
			[&app, this]()
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
					if (ImGui::MenuItem("New Canvas"))
					{
						m_UIManager->OpenResizeCanvasModal();
					}

					if (ImGui::MenuItem("Undo (ctrl+z)"))
					{
						m_CanvasCoordinator->UndoHistory();
					}

					if (ImGui::MenuItem("Redo (ctrl+y)"))
					{
						m_CanvasCoordinator->RedoHistory();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Default Window Layout"))
					{
						m_UIManager->SetDefaultLayout();
					}

					if (ImGui::MenuItem("Centre Canvas"))
					{
						m_CanvasViewport->SetNeedsCentering();
					}

					ImGui::EndMenu();
				}
			}
		);
	}

	bool EditorLayer::OnKeyPressed(Walnut::KeyPressedEvent& event)
	{
		const Walnut::KeyCode keyCode = event.GetKeyCode();

		// Input management
		if (keyCode == Walnut::KeyCode::LeftControl || keyCode == Walnut::KeyCode::RightControl)
		{
			m_CtrlDown = true;
		}

		if (keyCode == Walnut::KeyCode::Space && !event.IsRepeat())
		{
			m_SpaceDown = true;
		}

		// Actions
		if (auto toolEnum = m_ToolRegistry->GetToolEnum(keyCode); toolEnum != ToolEnum::None)
		{
			m_ToolManager->SetTool(toolEnum);
			return true;
		}

		if (m_CtrlDown && m_CanvasCoordinator->OnKeyPressed(keyCode)) return true;
		if (m_ToolSettingsController->OnKeyPressed(keyCode)) return true;

		return false;
	}

	bool EditorLayer::OnKeyReleased(Walnut::KeyReleasedEvent& event)
	{
		const Walnut::KeyCode keyCode = event.GetKeyCode();

		// Input management
		if (keyCode == Walnut::KeyCode::LeftControl || keyCode == Walnut::KeyCode::RightControl)
		{
			m_CtrlDown = false;
		}

		if (keyCode == Walnut::KeyCode::Space)
		{
			m_SpaceDown = false;
		}

		return false;
	}

	bool EditorLayer::OnMousePressed(Walnut::MousePressedEvent& event)
	{
		const Walnut::MouseButton mouseButton = event.GetMouseButton();

		// Input management
		if (mouseButton == Walnut::MouseButton::Left)
		{
			m_LeftMouseDown = true;
		}

		if (mouseButton == Walnut::MouseButton::Right)
		{
			m_RightMouseDown = true;
		}

		return false;
	}

	bool EditorLayer::OnMouseReleased(Walnut::MouseReleasedEvent& event)
	{
		const Walnut::MouseButton mouseButton = event.GetMouseButton();

		// Input management
		if (mouseButton == Walnut::MouseButton::Left)
		{
			m_LeftMouseDown = false;
		}

		if (mouseButton == Walnut::MouseButton::Right)
		{
			m_RightMouseDown = false;
		}

		// Actions
		if (m_ShouldAddToHistory)
		{
			m_CanvasCoordinator->SaveHistory();
			m_ShouldAddToHistory = false;
		}

		return false;
	}

	bool EditorLayer::OnMouseScrolled(Walnut::MouseScrolledEvent& event)
	{
		const glm::vec2 scrollOffset = event.GetScrollOffset();

		// Actions
		if (m_CtrlDown)
		{
			m_CanvasCoordinator->OnMouseScrolled(scrollOffset, m_TimeStep);
			return true;
		}

		return false;
	}

	bool EditorLayer::OnMouseMoved(Walnut::MouseMovedEvent& event)
	{
		glm::vec2 prevScreenMousePos = m_ScreenMousePos;
		m_ScreenMousePos = event.GetMousePos();

		// Actions
		if (m_SpaceDown && (m_LeftMouseDown || m_RightMouseDown))
		{
			m_CanvasCoordinator->OnMouseMoved(m_ScreenMousePos, prevScreenMousePos, m_TimeStep);
			return true;
		}

		return false;
	}

}
