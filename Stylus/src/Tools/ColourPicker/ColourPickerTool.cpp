#include "ColourPickerTool.h"

#include "../../Layers/CanvasLayer.h"
#include "../../Utils/CanvasUtils.h"
#include <Walnut/Application.h>

namespace Stylus {

	ColourPickerTool::ColourPickerTool(std::function<void()> drawUiStrategy, ToolData toolData, ColourPickerSettingsContext context)
		: Tool(drawUiStrategy, toolData), m_SettingsContext(context) {}

    bool ColourPickerTool::UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
    {
        glm::vec4 colour = Utils::GetColourAt(mousePos);
        m_SettingsContext.Set<TSE::PrimaryColour>(colour);

        return false;
    }

	bool ColourPickerTool::UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
	{
        glm::vec4 colour = Utils::GetColourAt(mousePos);
        m_SettingsContext.Set<TSE::SecondaryColour>(colour);

        return false;
    }

    void ColourPickerTool::DrawOverlayHint(ImVec2 mousePos) const
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), 1, s_OverlayHintColour, 50, s_OverlayHintThickness);
    }

}
