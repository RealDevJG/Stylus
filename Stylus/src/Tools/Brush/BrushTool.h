#pragma once

#include "../../Systems/ToolSettingsStore.h"
#include "../Tool.h"

namespace Stylus {

	class BrushTool final : public Tool
	{
	public:
		BrushTool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy, ToolSettingsStore& settingsStore);

		ToolAction GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		ToolAction GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(const glm::vec2 mousePos, float scale) const override;
	private:
		ToolSettingsStore& m_SettingsStore;
	};

}
