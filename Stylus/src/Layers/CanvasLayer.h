#pragma once

#include <Walnut/Application.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <glm/glm.hpp>
#include <memory>

namespace Stylus {

	class ToolManager;
	class ShaderRegistry;

	class CanvasLayer final : public Walnut::Layer
	{
	public:
		CanvasLayer(std::shared_ptr<ToolManager> toolManager, std::shared_ptr<ShaderRegistry> shaderRegistry)
			: m_ToolManager(toolManager), m_ShaderRegistry(shaderRegistry) {}
		CanvasLayer(std::shared_ptr<ToolManager> toolManager, std::shared_ptr<ShaderRegistry> shaderRegistry, uint32_t canvasWidth, uint32_t canvasHeight)
			: m_ToolManager(toolManager), m_ShaderRegistry(shaderRegistry), m_CanvasWidth(canvasWidth), m_CanvasHeight(canvasHeight) {}

		virtual void OnAttach() override;
		virtual void OnEvent(Walnut::Event& event) override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float ts) override;

		bool OnMousePressed(Walnut::MousePressedEvent& event);
		bool OnMouseReleased(Walnut::MouseReleasedEvent& event);

		std::shared_ptr<Walnut::Image> GetCanvasImage() const { return m_CanvasImage; }
	private:
		std::shared_ptr<Walnut::Image> m_CanvasImage{};

		glm::vec2 m_MousePos{};
		glm::vec2 m_PrevMousePos{};
		uint32_t m_CanvasWidth = 854;
		uint32_t m_CanvasHeight = 480;

		bool m_CanvasHovered = false;
		bool m_LeftMouseDown = false;
		bool m_RightMouseDown = false;

		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ShaderRegistry> m_ShaderRegistry;
	};

}
