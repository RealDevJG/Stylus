#pragma once

#include "../CoreContext.h"

#include <Walnut/Application.h>
#include <memory>

class ApplicationLayer final : public Walnut::Layer
{
public:
	ApplicationLayer(std::shared_ptr<Stylus::CoreContext> context)
		: m_Context(context) {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUIRender() override;

	void UI_DrawAboutModal();
	void ShowAboutModal();
private:
	bool m_AboutModalOpen = false;
	std::shared_ptr<Stylus::CoreContext> m_Context;
};
