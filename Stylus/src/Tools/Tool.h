#pragma once

#include "ToolData.h"

#include <functional>
#include <glm/glm.hpp>

namespace Stylus {

	class Tool
	{
	public:
		Tool(std::function<void()> drawUiStrategy, ToolData toolData)
			: m_DrawUiStrategy(drawUiStrategy), m_ToolData(toolData) {}

		void DrawOptionsUI() const { m_DrawUiStrategy(); }

		virtual void UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const = 0;
		virtual void UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const = 0;

		[[nodiscard]] const ToolData GetToolData() const { return m_ToolData; }
	private:
		std::function<void()> m_DrawUiStrategy;
		ToolData m_ToolData;
	};

}
