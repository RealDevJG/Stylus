#pragma once

#include "../Systems/HistoryManager.h"
#include "../Ui/CanvasViewport.h"

#include <Walnut/Application.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Stylus {

	class ToolManager;
	class ShaderRegistry;

	class CanvasLayer final : public Walnut::Layer
	{
	public:
		CanvasLayer(
			std::shared_ptr<ToolManager> toolManager,
			std::shared_ptr<ShaderRegistry> shaderRegistry,
			std::shared_ptr<HistoryManager<std::vector<uint8_t>>> historyManager
		) : m_ToolManager(toolManager), m_ShaderRegistry(shaderRegistry), m_HistoryManager(historyManager) {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Walnut::Event& event) override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float ts) override;

		void ClearHistory();
		void SaveHistory();
		void UndoHistory();
		void RedoHistory();

		bool OnKeyPressed(Walnut::KeyPressedEvent& event);
		bool OnKeyReleased(Walnut::KeyReleasedEvent& event);
		bool OnMousePressed(Walnut::MousePressedEvent& event);
		bool OnMouseReleased(Walnut::MouseReleasedEvent& event);
		bool OnMouseScrolled(Walnut::MouseScrolledEvent& event);

		void CreateCanvas(uint32_t width, uint32_t height);
		void ResizeCanvas(uint32_t width, uint32_t height);
		float GetCanvasScale();

		void SetCanvasData(const void* data) const;

		std::shared_ptr<Walnut::Image> GetCanvasImage() const { return m_CanvasImage; }
	private:
		void UpdateMousePos();
	private:
		CanvasViewport m_CanvasViewport;
		std::shared_ptr<Walnut::Image> m_CanvasImage{};

		glm::vec2 m_MousePos{};
		glm::vec2 m_PrevMousePos{};

		bool m_ViewportHovered = false;
		bool m_ShouldAddToHistory = false;

		bool m_CtrlDown = false;
		bool m_SpaceDown = false;
		bool m_LeftMouseDown = false;
		bool m_RightMouseDown = false;

		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ShaderRegistry> m_ShaderRegistry;
		std::shared_ptr<HistoryManager<std::vector<uint8_t>>> m_HistoryManager;
	};

}
