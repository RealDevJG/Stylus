#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../../Systems/ToolSettingsProvider.h"

namespace Stylus {

	using ColourPickerSettingsContext = ToolSettingsProvider<TSE::PrimaryColour, TSE::SecondaryColour>;

	class ColourPickerTool final : public Tool
	{
	public:
		ColourPickerTool(std::function<void()> drawStrategy, ToolData toolData, ColourPickerSettingsContext context);

		bool UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		bool UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(ImVec2 mousePos) const override;
	private:
		ColourPickerSettingsContext m_SettingsContext;
	};

}
