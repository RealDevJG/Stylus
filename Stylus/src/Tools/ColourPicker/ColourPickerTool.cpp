#include "ColourPickerTool.h"

#include "../../Layers/CanvasLayer.h"
#include "../../Utils/CanvasUtils.h"

#include <Walnut/Application.h>

namespace Stylus {

	ColourPickerTool::ColourPickerTool(std::function<void()> drawUiStrategy, const ToolData& toolData, ColourPickerSettingsContext context)
		: Tool(drawUiStrategy, toolData), m_SettingsContext(context) {}

    bool ColourPickerTool::UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        glm::vec4 colour = Utils::GetColourAt(mousePos);
        m_SettingsContext.Set<TSE::PrimaryColour>(colour);
        return false;
    }

	bool ColourPickerTool::UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
        glm::vec4 colour = Utils::GetColourAt(mousePos);
        m_SettingsContext.Set<TSE::SecondaryColour>(colour);
        return false;
    }

    void ColourPickerTool::DrawOverlayHint(const ImVec2 mousePos, float scale) const
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), 0.5f * scale, s_OverlayHintColour, 50, s_OverlayHintThickness);
    }

}
