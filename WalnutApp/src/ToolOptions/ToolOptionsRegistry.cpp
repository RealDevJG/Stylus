#include "ToolOptionsRegistry.h"

#include "Brush/BrushPushData.h"

namespace Stylus {

	ToolOptionsRegistry::ToolOptionsRegistry()
	{
        m_ToolMap[ToolEnum::Brush] = ToolHooks{
            .DrawUI = [this]() {
                // ImGui::SliderFloat("Radius", &m_SharedRadius, 1, 100);
                // ImGui::ColorPicker4("Color", &m_PrimaryColour.x);
            },
            .Dispatch = [this](ComputePipeline* shader, glm::vec2 mousePos) {
                BrushPushData pushData{
                    .Colour = m_PrimaryColour,
                    .MousePos = mousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased
                };

                shader->DispatchShader(&pushData);
            }
        };
	}

}
