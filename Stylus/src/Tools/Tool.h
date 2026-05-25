#pragma once

#include "ToolActions.h"
#include "ToolData.h"

#include <glm/vec2.hpp>
#include <functional>

namespace Stylus {

	class Tool
	{
	public:
		Tool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy)
			: m_ToolData(toolData), m_DrawSettingsUIStrategy(drawSettingsUIStrategy) {}

		void DrawSettingsUI() const { m_DrawSettingsUIStrategy(); }

		[[nodiscard]] virtual ToolAction GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		[[nodiscard]] virtual ToolAction GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		[[nodiscard]] virtual ToolAction DrawOverlayHint(const glm::vec2 mousePos, float scale) const = 0;

		[[nodiscard]] const ToolData& GetToolData() const { return m_ToolData; }
	private:
		ToolData m_ToolData;
		std::function<void()> m_DrawSettingsUIStrategy;
	};

}
