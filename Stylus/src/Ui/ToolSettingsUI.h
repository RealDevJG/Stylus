#pragma once

#include "../Tools/Brush/BrushShape.h"
#include "../Tools/ToolSettingsEnum.h"
#include "EnumSelection.h"

#include <glm/vec4.hpp>
#include <imgui.h>

namespace Stylus {

	class ToolSettingsStore;

	namespace UI::ToolSettings {

		constexpr auto GetPrimaryColourWidget(glm::vec4& primaryColour)
		{
			return [&primaryColour]() { ImGui::ColorEdit4("Primary Colour", &primaryColour.x, ImGuiColorEditFlags_NoInputs); };
		}

		constexpr auto GetSecondaryColourWidget(glm::vec4& secondaryColour)
		{
			return [&secondaryColour]() { ImGui::ColorEdit4("Secondary Colour", &secondaryColour.x, ImGuiColorEditFlags_NoInputs); };
		}

		constexpr auto GetBrushWidthWidget(float& currentWidth)
		{
			return [&currentWidth]() { ImGui::SliderFloat("Width", &currentWidth, 1, 256); };
		}

		constexpr auto GetBrushShapeWidget(BrushShapeEnum& brushShape)
		{
			return [&brushShape]() { UI::EnumSelection("Brush Shape", brushShape, BrushShapeList); };
		}

		constexpr auto GetAntialiasingWidget(bool& antialiasingEnabled)
		{
			return [&antialiasingEnabled]() { ImGui::Checkbox("Antialiasing", &antialiasingEnabled); };
		}

		template<ToolSettingsEnum Setting>
		constexpr auto GetSettingWidget(ToolSettingsStore& settingsStore)
		{
			auto& value = settingsStore.GetValue<Setting>();

			if constexpr (Setting == ToolSettingsEnum::PrimaryColour) return GetPrimaryColourWidget(value);
			else if constexpr (Setting == ToolSettingsEnum::SecondaryColour) return GetSecondaryColourWidget(value);
			else if constexpr (Setting == ToolSettingsEnum::Width) return GetBrushWidthWidget(value);
			else if constexpr (Setting == ToolSettingsEnum::Shape) return GetBrushShapeWidget(value);
			else if constexpr (Setting == ToolSettingsEnum::Antialiasing) return GetAntialiasingWidget(value);
			else static_assert(false, "GetSettingWidget: Unhandled ToolSettingsEnum value");
		}

		template<typename... UIControls>
		constexpr void DrawSettingsBar(UIControls&&... packedControls)
		{
			ImGui::PushItemWidth(120);
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.9f, 0.25f));

			size_t totalElements = sizeof...(UIControls);
			size_t currentIndex = 0;

			([&] {
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

		template<ToolSettingsEnum... Settings>
		constexpr auto CreateToolbarSettingsDrawer(ToolSettingsStore& settingsStore)
		{
			return [&settingsStore]() {
				DrawSettingsBar(GetSettingWidget<Settings>(settingsStore)...);
			};
		}

	}

}
