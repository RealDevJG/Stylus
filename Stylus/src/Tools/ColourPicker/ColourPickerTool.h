#pragma once

#include "../../Systems/ToolSettingsStore.h"
#include "../Tool.h"

namespace Stylus {

	class ColourPickerTool final : public Tool
	{
	public:
		ColourPickerTool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy, ToolSettingsStore& settingsStore);

		[[nodiscard]] ToolAction GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		[[nodiscard]] ToolAction GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		[[nodiscard]] ToolAction DrawOverlayHint(const glm::vec2 mousePos, float scale) const override;
	private:
		ToolSettingsStore& m_SettingsStore;
	};

}
