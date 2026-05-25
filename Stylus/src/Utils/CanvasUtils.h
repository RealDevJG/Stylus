#pragma once

#include "../Layers/EditorLayer.h"

#include <glm/glm.hpp>
#include <Walnut/Application.h>

namespace Stylus::Utils {

    [[nodiscard]] const glm::vec4 GetColourAt(glm::vec2 mousePos)
    {
        auto editorLayer = Walnut::Application::Get().GetLayer<EditorLayer>();
        const Walnut::Image& canvasImage = editorLayer->GetCanvasImage();

        uint32_t x = glm::clamp((uint32_t)mousePos.x, 0u, canvasImage.GetWidth() - 1);
        uint32_t y = glm::clamp((uint32_t)mousePos.y, 0u, canvasImage.GetHeight() - 1);

        std::vector<uint8_t> pixelData = canvasImage.ReadPixels(x, y, 1, 1);

        if (!pixelData.empty())
        {
            return glm::vec4{
                pixelData[0] / 255.0f,
                pixelData[1] / 255.0f,
                pixelData[2] / 255.0f,
                pixelData[3] / 255.0f
            };
        }

        return glm::vec4{ 0.0f };
    }

}
