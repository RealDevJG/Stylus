#pragma once

#include "../Concepts/EnumType.h"
#include "../Concepts/TupleLikeType.h"

#include <span>
#include <ranges>
#include <imgui.h>

namespace Stylus::UI {

    template<Concepts::EnumType Enum, std::ranges::input_range Range>
        requires Concepts::TupleLikeType<std::ranges::range_reference_t<Range>>
    constexpr bool EnumSelection(const char* label, Enum& currentValue, Range&& items)
    {
        const char* preview = "";

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

}
