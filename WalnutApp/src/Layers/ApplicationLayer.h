#pragma once

#include "../CoreContext.h"

#include <Walnut/Application.h>
#include <memory>

namespace Stylus {

	class ApplicationLayer final : public Walnut::Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void UI_DrawAboutModal();
		void ShowAboutModal();
	private:
		bool m_AboutModalOpen = false;
		std::unique_ptr<CoreContext> m_CoreContext;
	};

}
