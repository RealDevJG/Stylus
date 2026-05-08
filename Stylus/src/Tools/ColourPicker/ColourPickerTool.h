#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../../Systems/ToolSettingsProvider.h"

namespace Stylus {

	using ColourPickerSettingsContext = ToolSettingsProvider<TSE::PrimaryColour, TSE::SecondaryColour>;

	class ColourPickerTool final : public Tool
	{
	public:
		ColourPickerTool(std::function<void()> drawStrategy, const ToolData& toolData, ColourPickerSettingsContext context);

		bool UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		bool UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(const ImVec2 mousePos, float scale) const override;
	private:
		ColourPickerSettingsContext m_SettingsContext;
	};

}
