#pragma once

#include "../Tool.h"

namespace Stylus {

	class ColourPickerTool final : public Tool
	{
	public:
		ColourPickerTool(const ToolData& toolData, std::function<void()> drawStrategy);

		ToolAction GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		ToolAction GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(const glm::vec2 mousePos, float scale) const override;
	};

}
