#pragma once

#include "../Systems/IToolManagerState.h"
#include "../Systems/IToolRegistryReadonly.h"
#include "../Tools/ToolActionExecutor.h"
#include "../Tools/ToolEnum.h"
#include "../UI/ICanvasContext.h"

#include <array>
#include <string_view>

namespace Stylus {

	class UIManager
	{
	public:
		UIManager(IToolManagerState& toolManagerState, IToolRegistryReadonly& toolRegistry, ICanvasContext& canvasContext, const ToolActionExecutor& actionExecutor);
		~UIManager() = default;

		UIManager(const UIManager&) = delete;
		UIManager& operator=(const UIManager&) = delete;
		UIManager(UIManager&&) = delete;
		UIManager& operator=(UIManager&&) = delete;

		void Render();

		void SetDefaultLayout();
		void OpenResizeCanvasModal();
	private:
		void DefaultLayout() const;
		void DrawResizeCanvasModal();
		void DrawToolBar() const;
		void DrawToolSettings() const;
		void DrawToolOverlayHint() const;
		void DrawToolButton(ToolEnum toolEnum, std::string_view toolName) const;
	private:
		IToolManagerState& m_ToolManagerState;
		IToolRegistryReadonly& m_ToolRegistryReadonly;
		ICanvasContext& m_CanvasContext;
		const ToolActionExecutor& m_ActionExecutor;

		std::array<char, 5> m_ResizeWidthBuffer{};
		std::array<char, 5> m_ResizeHeightBuffer{};

		bool m_ShouldSetDefaultLayout = false;
		bool m_ResizeCanvasModalOpen = false;
		bool m_ShouldCentreResizeModal = false;
	};

}
