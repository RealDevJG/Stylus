#pragma once

#include "ToolData.h"

#include <glm/glm.hpp>
#include <imgui.h>
#include <functional>

namespace Stylus {

	class Tool
	{
	public:
		inline static ImU32 s_OverlayHintColour = IM_COL32(0, 0, 0, 255);
		inline static float s_OverlayHintThickness = 1.0f;

		Tool(std::function<void()> drawUiStrategy, const ToolData& toolData)
			: m_DrawUiStrategy(drawUiStrategy), m_ToolData(toolData) {}

		void DrawOptionsUI() const { m_DrawUiStrategy(); }

		virtual bool UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		virtual bool UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		virtual void DrawOverlayHint(const ImVec2 mousePos, float scale) const = 0;

		[[nodiscard]] const ToolData& GetToolData() const { return m_ToolData; }
	private:
		std::function<void()> m_DrawUiStrategy;
		ToolData m_ToolData;
	};

}
