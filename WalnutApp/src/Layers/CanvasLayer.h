#pragma once

#include <Walnut/Application.h>

class CanvasLayer final : public Walnut::Layer
{
public:
	virtual void OnUIRender() override;
	virtual void OnUpdate(float ts) override;
private:
	std::shared_ptr<Walnut::Image> m_CanvasData;
};
