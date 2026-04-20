#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../ToolSettingsProvider.h"

namespace Stylus {

	using ColourPickerSettingsContext = ToolSettingsProvider<TSE::PrimaryColour, TSE::SecondaryColour>;

	class ColourPickerTool final : public Tool
	{
	public:
		ColourPickerTool(std::function<void()> drawStrategy, ToolData toolData, ColourPickerSettingsContext context);

		void UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		void UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
	private:
		ColourPickerSettingsContext m_SettingsContext;
	};

}
