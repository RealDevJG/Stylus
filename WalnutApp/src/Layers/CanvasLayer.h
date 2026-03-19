#pragma once

#include <Walnut/Application.h>

class CanvasLayer final : public Walnut::Layer
{
public:
	CanvasLayer() = delete;
	CanvasLayer(uint32_t canvasWidth, uint32_t canvasHeight);

	virtual void OnAttach() override;
	virtual void OnUIRender() override;
private:
	bool IsInBounds(float x, float y);
	void DrawWithRadius(int32_t centreX, int32_t centreY, int radius, uint32_t colour);
private:
	std::shared_ptr<Walnut::Image> m_CanvasImage;
	std::unique_ptr<uint32_t[]> m_CanvasData;

	uint32_t m_CanvasHeight;
	uint32_t m_CanvasWidth;
};
