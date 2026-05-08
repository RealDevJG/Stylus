#pragma once

#include "../Ui/UIDrawer.h"
#include <Walnut/Layer.h>

#include <array>
#include <memory>

namespace Stylus {

	class ToolManager;
	class ToolRegistry;

	class UiLayer final : public Walnut::Layer
	{
	public:
		UiLayer(std::shared_ptr<ToolManager> toolManager, std::shared_ptr<ToolRegistry> toolRegistry)
			: m_ToolManager(toolManager), m_ToolRegistry(toolRegistry), m_UiDrawer(toolManager.get()) {}

		virtual void OnUIRender() override;

		void SetDefaultLayout();
		void OpenResizeCanvasModal();
	private:
		void DefaultLayout() const;
		void DrawResizeCanvasModal();
		void DrawToolBar() const;
		void DrawToolSettings() const;
	private:
		UIDrawer m_UiDrawer;
		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ToolRegistry> m_ToolRegistry;

		std::array<char, 5> m_ResizeWidthBuffer{};
		std::array<char, 5> m_ResizeHeightBuffer{};

		bool m_ShouldSetDefaultLayout = false;
		bool m_ResizeCanvasModalOpen = false;
		bool m_ShouldCentreResizeModal = false;
	};

}
