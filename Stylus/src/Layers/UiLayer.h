#pragma once

#include "../Ui/UIDrawer.h"
#include <Walnut/Application.h>

#include <memory>

namespace Stylus {

	class ToolManager;
	class ToolRegistry;

	class UiLayer final : public Walnut::Layer
	{
	public:
		UiLayer(std::shared_ptr<ToolManager> toolManager, std::shared_ptr<ToolRegistry> toolRegistry)
			: m_ToolManager(toolManager), m_ToolRegistry(toolRegistry), m_UiDrawer(toolManager) {}

		virtual void OnUIRender() override;
	private:
		UIDrawer m_UiDrawer;

		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ToolRegistry> m_ToolRegistry;
	};

}
