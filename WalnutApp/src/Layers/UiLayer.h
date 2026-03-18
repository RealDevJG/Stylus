#pragma once

#include <Walnut/Application.h>

class UiLayer final : public Walnut::Layer
{
	virtual void OnUIRender() override;
};
