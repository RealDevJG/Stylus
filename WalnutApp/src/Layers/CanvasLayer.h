#pragma once

#include "../CoreContext.h"
#include "../Vulkan/ComputePipeline.h"

#include <glm/glm.hpp>
#include <Walnut/Application.h>

#include <memory>

class CanvasLayer final : public Walnut::Layer
{
public:
	CanvasLayer(std::shared_ptr<Stylus::CoreContext> context);
	CanvasLayer(std::shared_ptr<Stylus::CoreContext> context, uint32_t canvasWidth, uint32_t canvasHeight);

	virtual void OnAttach() override;
	virtual void OnUIRender() override;
	virtual void OnUpdate(float ts) override;
private:
	std::shared_ptr<Walnut::Image> m_CanvasImage{};
	std::shared_ptr<Stylus::CoreContext> m_Context{};

	glm::vec2 m_MousePos{};
	glm::vec2 m_PrevMousePos{};
	uint32_t m_CanvasWidth = 854;
	uint32_t m_CanvasHeight = 480;

	bool m_IsCanvasHovered = false;
	bool m_LeftMouseDown = false;
	bool m_RightMouseDown = false;
};
