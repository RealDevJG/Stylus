#pragma once

#include <imgui.h>

namespace Stylus::UI {

    template<typename... UIControls>
    constexpr void DrawToolbarRow(UIControls&&... packedControls)
    {
        ImGui::PushItemWidth(120);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.9f, 0.25f));

        size_t totalElements = sizeof...(UIControls);
        size_t currentIndex = 0;

        ([&]
        {
            packedControls();
            currentIndex++;

            if (currentIndex < totalElements)
            {
                ImGui::SameLine(0.0f, 25.0f);
            }
        }(), ...);

        ImGui::PopStyleColor(2);
        ImGui::PopItemWidth();
    }

}
