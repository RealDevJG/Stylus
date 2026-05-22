#pragma once

#include "../Systems/HistoryManager.h"
#include "../Systems/IShaderImageUpdater.h"
#include "../Tools/ToolActionExecutor.h"

#include "CanvasViewport.h"
#include "Canvas.h"
#include "ICanvasContext.h"

#include <glm/vec2.hpp>
#include <Walnut/Input/KeyCodes.h>

#include <cstdint>
#include <vector>

namespace Stylus {

	class CanvasCoordinator final : public ICanvasContext
	{
	public:
		CanvasCoordinator(Canvas& canvas, CanvasViewport& canvasViewport, HistoryManager<std::vector<uint8_t>>& historyManager, const ToolActionExecutor& toolExecutor, IShaderImageUpdater& shaderImageUpdater);
		~CanvasCoordinator() = default;

		CanvasCoordinator(const CanvasCoordinator&) = delete;
		CanvasCoordinator& operator=(const CanvasCoordinator&) = delete;
		CanvasCoordinator(CanvasCoordinator&&) = delete;
		CanvasCoordinator&& operator=(CanvasCoordinator&&) = delete;

		void CreateCanvas(uint32_t width, uint32_t height) override;
		void Render();

		void ClearHistory();
		void SaveHistory();
		void UndoHistory();
		void RedoHistory();

		[[nodiscard]] bool OnKeyPressed(Walnut::KeyCode keyCode);
		void OnMouseScrolled(glm::vec2 scrollOffset, float ts);
		void OnMouseMoved(glm::vec2 screenMousePos, glm::vec2 prevScreenMousePos, float ts);

		[[nodiscard]] bool IsCanvasViewportHovered() const;
		[[nodiscard]] glm::vec2 GetCanvasMousePos() const;
		[[nodiscard]] float GetCanvasScale() const override;
	private:
		void UpdateMousePos();
	private:
		Canvas& m_Canvas;
		CanvasViewport& m_CanvasViewport;
		HistoryManager<std::vector<uint8_t>>& m_HistoryManager;
		const ToolActionExecutor& m_ActionExecutor;
		IShaderImageUpdater& m_ShaderImageUpdater;

		glm::vec2 m_MousePos{};
		glm::vec2 m_PrevMousePos{};
		bool m_ViewportHovered = false;
	};

}
