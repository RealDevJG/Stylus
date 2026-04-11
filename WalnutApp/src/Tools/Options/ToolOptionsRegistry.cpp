#include "ToolOptionsRegistry.h"

#include "Brush/BrushPushData.h"
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
                    .Colour = m_PrimaryColour,
                    .MousePos = mousePos,
                    .PrevMousePos = prevMousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            },
            .RightClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos) {
                BrushPushData pushData{
                    .Colour = m_SecondaryColour,
                    .MousePos = mousePos,
                    .PrevMousePos = prevMousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            }
        };

        m_ToolMap[ToolEnum::Eraser] = ToolHooks{
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
                    .Colour = m_PrimaryColour,
                    .MousePos = mousePos,
                    .PrevMousePos = prevMousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            },
            .RightClickDispatch = [this](ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos) {
                BrushPushData pushData{
                    .Colour = m_SecondaryColour,
                    .MousePos = mousePos,
                    .PrevMousePos = prevMousePos,
                    .Shape = static_cast<int>(m_BrushShape),
                    .Radius = m_BrushWidth,
                    .Antialiased = m_Antialiased ? 1u : 0u
                };

                shader->DispatchShader(&pushData);
            }
        };
	}

    // TODO: temporarily here until better tool management
    std::vector<ToolEnum> ToolOptionsRegistry::GetTools()
    {
        std::vector<ToolEnum> tools(m_ToolMap.size());
        std::transform(begin(m_ToolMap), end(m_ToolMap), begin(tools), [](auto pair) { return pair.first; });

        return tools;
    }

}
