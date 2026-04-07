#include "ToolOptionsRegistry.h"

#include "Brush/BrushPushData.h"
#include "../../Ui/DrawUiRow.h"
#include "../../Ui/EnumSelection.h"

#include <imgui.h>

namespace Stylus {

	ToolOptionsRegistry::ToolOptionsRegistry()
	{
        m_ToolMap[ToolEnum::Brush] = ToolHooks{
            .DrawUI = [this]() {
                DrawToolbarRow(
                    [&]() { ImGui::SliderFloat("Width", &m_BrushWidth, 1, 100); },
                    [&]() { EnumSelection("Brush Shape", m_BrushShape, BrushShapeList); },
                    [&]() { ImGui::ColorEdit4("Primary Colour", &m_PrimaryColour.x, ImGuiColorEditFlags_NoInputs); },
                    [&]() { ImGui::ColorEdit4("Secondary Colour", &m_SecondaryColour.x, ImGuiColorEditFlags_NoInputs); },
                    [&]() { ImGui::Checkbox("Antialiasing", &m_Antialiased); }
                );
            },
            .LeftClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos) {
                BrushPushData pushData{
                    .Colour = m_PrimaryColour,
                    .MousePos = mousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            },
            .RightClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos) {
                BrushPushData pushData{
                    .Colour = m_SecondaryColour,
                    .MousePos = mousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            }
        };
	}

}
