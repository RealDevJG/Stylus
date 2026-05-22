#pragma once

#include "ToolActions.h"
#include "ToolData.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp> // TODO: remove when below TODO is moved too
#include <functional>

namespace Stylus {

	class Tool
	{
	public:
		// TODO: move
		inline static glm::vec4 s_OverlayHintColour{ 0.0f, 0.0f, 0.0f, 255.0f };
		inline static float s_OverlayHintThickness = 1.0f;

		Tool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy)
			: m_ToolData(toolData), m_DrawSettingsUIStrategy(drawSettingsUIStrategy) {}

		void DrawSettingsUI() const { m_DrawSettingsUIStrategy(); }

		virtual ToolAction GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		virtual ToolAction GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		virtual void DrawOverlayHint(const glm::vec2 mousePos, float scale) const = 0;

		[[nodiscard]] const ToolData& GetToolData() const { return m_ToolData; }
	private:
		ToolData m_ToolData;
		std::function<void()> m_DrawSettingsUIStrategy;
	};

}
