#pragma once

#include <glm/glm.hpp>
#include <Walnut/Application.h>
#include <memory>

namespace Stylus {

	class CanvasLayer final : public Walnut::Layer
	{
	public:
		CanvasLayer() = default;
		CanvasLayer(uint32_t canvasWidth, uint32_t canvasHeight);

		virtual void OnAttach() override;
		virtual void OnUIRender() override;
		virtual void OnUpdate(float ts) override;

		std::shared_ptr<Walnut::Image> GetCanvasImage() const { return m_CanvasImage; }
	private:
		std::shared_ptr<Walnut::Image> m_CanvasImage{};

		glm::vec2 m_MousePos{};
		glm::vec2 m_PrevMousePos{};
		uint32_t m_CanvasWidth = 854;
		uint32_t m_CanvasHeight = 480;

		bool m_IsCanvasHovered = false;
		bool m_LeftMouseDown = false;
		bool m_RightMouseDown = false;
	};

}
