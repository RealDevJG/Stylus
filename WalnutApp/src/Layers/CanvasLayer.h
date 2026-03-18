#pragma once

#include <Walnut/Application.h>

class CanvasLayer final : public Walnut::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnUIRender() override;
private:
	std::shared_ptr<Walnut::Image> m_CanvasData;
};
