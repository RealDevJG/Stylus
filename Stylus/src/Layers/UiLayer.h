#pragma once

#include "../Ui/UIDrawer.h"
#include <Walnut/Application.h>

#include <array>
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

		void SetDefaultLayout();
		void OpenResizeCanvasModal();
	private:
		void DefaultLayout();
		void DrawResizeCanvasModal();
	private:
		UIDrawer m_UiDrawer;

		std::array<char, 5> m_ResizeWidthBuffer{};
		std::array<char, 5> m_ResizeHeightBuffer{};
		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ToolRegistry> m_ToolRegistry;
		bool m_ShouldSetDefaultLayout = false;
		bool m_ResizeCanvasModalOpen = false;
		bool m_ShouldCentreResizeModal = false;
	};

}
