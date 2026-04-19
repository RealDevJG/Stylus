#include "ColourPickerTool.h"

#include "../../Layers/CanvasLayer.h"
#include <Walnut/Application.h>

namespace Stylus {

    // TODO: move to somewhere reusable
    glm::vec4 GetColourAt(glm::vec2 mousePos)
    {
        auto canvasLayer = Walnut::Application::Get().GetLayer<CanvasLayer>();
        auto canvasImage = canvasLayer->GetCanvasImage();

        uint32_t x = std::clamp((uint32_t)mousePos.x, 0u, canvasImage->GetWidth() - 1);
        uint32_t y = std::clamp((uint32_t)mousePos.y, 0u, canvasImage->GetHeight() - 1);

        std::vector<uint8_t> pixelData = canvasImage->ReadPixels(x, y, 1, 1);

        if (!pixelData.empty())
        {
            return glm::vec4(
                pixelData[0] / 255.0f,
                pixelData[1] / 255.0f,
                pixelData[2] / 255.0f,
                pixelData[3] / 255.0f
            );
        }

        return glm::vec4(999.0f);
    }

	ColourPickerTool::ColourPickerTool(std::function<void()> drawUiStrategy, ToolData toolData, ColourPickerSettingsContext context)
		: Tool(drawUiStrategy, toolData), m_SettingsContext(context) {}

    void ColourPickerTool::UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
    {
        glm::vec4 colour = GetColourAt(mousePos);
        m_SettingsContext.Set<TSE::PrimaryColour>(colour);
    }

	void ColourPickerTool::UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
	{
        glm::vec4 colour = GetColourAt(mousePos);
        m_SettingsContext.Set<TSE::SecondaryColour>(colour);
    }

}
