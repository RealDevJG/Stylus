#pragma once

#include "../Ui/UIDrawer.h"
#include <Walnut/Application.h>

namespace Stylus {

	class UiLayer final : public Walnut::Layer
	{
	public:
		virtual void OnUIRender() override;
	private:
		UIDrawer m_UiDrawer;
	};

}
