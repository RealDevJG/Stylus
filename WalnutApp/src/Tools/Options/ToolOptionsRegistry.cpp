#include "ToolOptionsRegistry.h"

#include "Brush/BrushPushData.h"
#include "Eraser/EraserPushData.h"
#include "../../Ui/DrawUiRow.h"
#include "../../Ui/EnumSelection.h"

#include <imgui.h>
#include <algorithm>

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
            .LeftClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos) {
                BrushPushData pushData{
                    m_PrimaryColour,
                    mousePos,
                    prevMousePos,
                    static_cast<int>(m_BrushShape),
                    m_BrushWidth,
                    m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            },
            .RightClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos) {
                BrushPushData pushData{
                    m_SecondaryColour,
                    mousePos,
                    prevMousePos,
                    static_cast<int>(m_BrushShape),
                    m_BrushWidth,
                    m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            }
        };

        m_ToolMap[ToolEnum::Eraser] = ToolHooks{
            .DrawUI = [this]() {
                DrawToolbarRow(
                    [&]() { ImGui::SliderFloat("Width", &m_BrushWidth, 1, 100); },
                    [&]() { EnumSelection("Brush Shape", m_BrushShape, BrushShapeList); },
                    [&]() { ImGui::Checkbox("Antialiasing", &m_Antialiased); }
                );
            },
            .LeftClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos) {
                EraserPushData pushData{
                    mousePos,
                    prevMousePos,
                    static_cast<int>(m_BrushShape),
                    m_BrushWidth,
                    m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            },
            .RightClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos) {
                EraserPushData pushData{
                    mousePos,
                    prevMousePos,
                    static_cast<int>(m_BrushShape),
                    m_BrushWidth,
                    m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            }
        };
	}

}
