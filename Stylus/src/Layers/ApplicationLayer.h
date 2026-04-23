#pragma once

#include "../CoreContext.h"

#include <Walnut/Application.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <memory>

namespace Stylus {

	class ApplicationLayer final : public Walnut::Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Walnut::Event& event) override;

		void UI_DrawAboutModal();
		void ShowAboutModal();
	private:
		bool OnKeyPressed(Walnut::KeyPressedEvent& event);
	private:
		bool m_AboutModalOpen = false;
		std::unique_ptr<CoreContext> m_CoreContext;
	};

}
