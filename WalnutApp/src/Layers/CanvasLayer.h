#pragma once

#include "../CoreContext.h"
#include "../Vulkan/ComputePipeline.h"

#include <Walnut/Application.h>
#include <memory>

class CanvasLayer final : public Walnut::Layer
{
public:
	CanvasLayer(std::shared_ptr<Stylus::CoreContext> context);
	CanvasLayer(std::shared_ptr<Stylus::CoreContext> context, uint32_t canvasWidth, uint32_t canvasHeight);

	virtual void OnAttach() override;
	virtual void OnUIRender() override;
private:
	bool IsInBounds(float x, float y);
private:
	std::shared_ptr<Walnut::Image> m_CanvasImage{};
	std::shared_ptr<Stylus::CoreContext> m_Context{};

	//std::unique_ptr<Stylus::ComputePipeline> m_ToolShader{};
	//std::unique_ptr<Stylus::ComputePipeline> m_CanvasFillShader{};

	uint32_t m_CanvasWidth = 854;
	uint32_t m_CanvasHeight = 480;
};
