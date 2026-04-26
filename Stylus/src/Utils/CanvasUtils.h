#pragma once

#include "../Layers/CanvasLayer.h"
#include <glm/glm.hpp>

namespace Stylus::Utils {

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

}
