#pragma once

#include <array>
#include <imgui.h>

template<typename Enum, size_t N>
bool EnumSelection(const char* label, Enum& currentValue, const std::array<std::pair<Enum, const char*>, N>& items)
{
    const char* preview = nullptr;

    for (const auto& [value, name] : items)
    {
        if (value == currentValue)
        {
            preview = name;
            break;
        }
    }

    bool changed = false;

    if (ImGui::BeginCombo(label, preview))
    {
        for (const auto& [value, name] : items)
        {
            bool isSelected = (value == currentValue);

            if (ImGui::Selectable(name, isSelected))
            {
                currentValue = value;
                changed = true;
            }

            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }

    return changed;
}
