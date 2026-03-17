#pragma once

#include <Walnut/Application.h>

class ApplicationLayer final : public Walnut::Layer
{
public:
	virtual void OnUIRender() override;

	void UI_DrawAboutModal();
	void ShowAboutModal();
private:
	bool m_AboutModalOpen = false;
};
