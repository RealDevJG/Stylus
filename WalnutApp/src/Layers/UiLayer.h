#pragma once

#include "../CoreContext.h"
#include <Walnut/Application.h>

class UiLayer final : public Walnut::Layer
{
public:
	UiLayer(std::shared_ptr<Stylus::CoreContext> context);

	virtual void OnUIRender() override;
private:
	std::shared_ptr<Stylus::CoreContext> m_Context;
};
