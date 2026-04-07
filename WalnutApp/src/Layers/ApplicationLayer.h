#pragma once

#include "../CoreContext.h"

#include <Walnut/Application.h>
#include <memory>
#include <iostream>

class ApplicationLayer final : public Walnut::Layer
{
public:
	ApplicationLayer(std::shared_ptr<Stylus::CoreContext> context)
		: m_Context(context)
	{
		std::cout << "Constructed\n";
	}
	~ApplicationLayer() { std::cout << "Destructed\n"; }

	ApplicationLayer(const ApplicationLayer&) { std::cout << "Copied\n"; }
	ApplicationLayer& operator=(const ApplicationLayer&) { std::cout << "Copy assigned\n"; }
	ApplicationLayer(ApplicationLayer&&) noexcept { std::cout << "Moved\n"; }
	ApplicationLayer& operator=(ApplicationLayer&&) noexcept { std::cout << "Move assigned\n"; }

	virtual void OnAttach() override;
	virtual void OnUIRender() override;

	void UI_DrawAboutModal();
	void ShowAboutModal();
private:
	bool m_AboutModalOpen = false;
	std::shared_ptr<Stylus::CoreContext> m_Context;
};
