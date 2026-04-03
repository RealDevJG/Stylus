#pragma once

#include "../Vulkan/ComputePipeline.h"
#include <Walnut/Application.h>

#include <memory>

class CanvasLayer final : public Walnut::Layer
{
public:
	CanvasLayer() = default;
	CanvasLayer(uint32_t canvasWidth, uint32_t canvasHeight);

	virtual void OnAttach() override;
	virtual void OnUIRender() override;
private:
	bool IsInBounds(float x, float y);
private:
	std::shared_ptr<Walnut::Image> m_CanvasImage{};

	std::unique_ptr<Stylus::ComputePipeline> m_ToolShader{};
	std::unique_ptr<Stylus::ComputePipeline> m_ColourFillShader{};

	uint32_t m_CanvasWidth = 854;
	uint32_t m_CanvasHeight = 480;
};
