#pragma once

#include "../Systems/ShaderRegistry.h"
#include "../Systems/HistoryManager.h"
#include "../Systems/ToolManager.h"
#include "../Systems/ToolRegistry.h"
#include "../Systems/ToolSettingsStore.h"
#include "../Systems/ToolSettingsController.h"
#include "../Systems/UIManager.h"

#include "../UI/CanvasCoordinator.h"
#include "../UI/CanvasViewport.h"

#include <Walnut/Layer.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <memory>
#include <vector>

namespace Stylus {

	class EditorLayer final : public Walnut::Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnUpdate(float ts) override;
		virtual void OnUIRender() override;
		virtual void OnEvent(Walnut::Event& event) override;

		[[nodiscard]] const Walnut::Image& GetCanvasImage() const { return m_Canvas->GetCanvasImage(); };
	private:
		void InitSystems();
		void SetMenubarCallback() const;

		bool OnKeyPressed(Walnut::KeyPressedEvent& event);
		bool OnKeyReleased(Walnut::KeyReleasedEvent& event);
		bool OnMousePressed(Walnut::MousePressedEvent& event);
		bool OnMouseReleased(Walnut::MouseReleasedEvent& event);
		bool OnMouseScrolled(Walnut::MouseScrolledEvent& event);
		bool OnMouseMoved(Walnut::MouseMovedEvent& event);
	private:
		std::unique_ptr<ShaderRegistry> m_ShaderRegistry;
		std::unique_ptr<ToolRegistry> m_ToolRegistry;
		std::unique_ptr<ToolSettingsStore> m_ToolSettingsStore;
		std::unique_ptr<ToolSettingsController> m_ToolSettingsController;
		std::unique_ptr<HistoryManager<std::vector<uint8_t>>> m_HistoryManager;
		std::unique_ptr<ToolManager> m_ToolManager;
		std::unique_ptr<CanvasViewport> m_CanvasViewport;
		std::unique_ptr<Canvas> m_Canvas;
		std::unique_ptr<ToolActionExecutor> m_ToolExecutor;
		std::unique_ptr<CanvasCoordinator> m_CanvasCoordinator;
		std::unique_ptr<UIManager> m_UIManager;

		glm::vec2 m_CanvasMousePos{};
		glm::vec2 m_ScreenMousePos{};

		float m_TimeStep = 0.0f;
		bool m_ShouldAddToHistory = false;

		// TODO: Create a central input manager - when making input manager, start using IToolManagerInteractions
		bool m_LeftMouseDown = false;
		bool m_RightMouseDown = false;
		bool m_CtrlDown = false;
		bool m_SpaceDown = false;
	};

}
