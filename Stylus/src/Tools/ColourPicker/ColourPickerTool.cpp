#include "ColourPickerTool.h"

#include "../../Utils/CanvasUtils.h"
#include "../../Systems/ToolSettingsUpdater.h"

#include <imgui.h>
#include <memory>

namespace Stylus {

	ColourPickerTool::ColourPickerTool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy)
		: Tool(toolData, drawSettingsUIStrategy) {}

    ToolAction ColourPickerTool::GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        glm::vec4 newColour = Utils::GetColourAt(mousePos);

        return ToolAction{
            ToolActionType::UpdateSettingValueAction,
            UpdateSettingValueAction{
                .SettingsUpdater = std::make_unique<ToolSettingsUpdater<TSE::PrimaryColour, glm::vec4>>(newColour)
            }
        };
    }

	ToolAction ColourPickerTool::GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
        glm::vec4 newColour = Utils::GetColourAt(mousePos);

        return ToolAction{
            ToolActionType::UpdateSettingValueAction,
            UpdateSettingValueAction{
                .SettingsUpdater = std::make_unique<ToolSettingsUpdater<TSE::SecondaryColour, glm::vec4>>(newColour)
            }
        };
    }

    void ColourPickerTool::DrawOverlayHint(const glm::vec2 mousePos, float scale) const
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), 0.5f * scale, IM_COL32(0, 0, 0, 255), 50, s_OverlayHintThickness);
    }

}
