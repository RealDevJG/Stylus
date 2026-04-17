#pragma once

#include "Brush/BrushShape.h"
#include "ToolSettingsEnum.h"
#include "../Ui/EnumSelection.h"
#include "../Ui/DrawUiRow.h"

#include <imgui.h>
#include <glm/glm.hpp>

namespace Stylus {

	template<auto T> inline constexpr bool dependent_false = false;

	class ToolOptionsRegistry
	{
	public:
		ToolOptionsRegistry() = default;
		~ToolOptionsRegistry() = default;

		ToolOptionsRegistry(const ToolOptionsRegistry&) = delete;
		ToolOptionsRegistry& operator=(const ToolOptionsRegistry&) = delete;
		ToolOptionsRegistry(ToolOptionsRegistry&&) = delete;
		ToolOptionsRegistry& operator=(ToolOptionsRegistry&&) = delete;

		template<ToolSettingsEnum Setting>
		const auto& GetValue()
		{
			if constexpr (Setting == ToolSettingsEnum::PrimaryColour) return m_PrimaryColour;
			else if constexpr (Setting == ToolSettingsEnum::SecondaryColour) return m_SecondaryColour;
			else if constexpr (Setting == ToolSettingsEnum::Width) return m_ToolWidth;
			else if constexpr (Setting == ToolSettingsEnum::Shape) return m_ToolShape;
			else if constexpr (Setting == ToolSettingsEnum::Antialiasing) return m_Antialiased;
			else static_assert(dependent_false<Setting>, "GetValue: Unhandled ToolSettingEnum value");
		}

		template<ToolSettingsEnum... Settings>
		auto CreateUiDrawer()
		{
			return [this]() {
				DrawToolbarRow(this->GetUiLambda<Settings>()...);
			};
		}
	private:
		auto PrimaryColourSettingsUi()
		{
			return [this]() { ImGui::ColorEdit4("Primary Colour", &m_PrimaryColour.x, ImGuiColorEditFlags_NoInputs); };
		}

		auto SecondaryColourSettingsUi()
		{
			return [this]() { ImGui::ColorEdit4("Secondary Colour", &m_SecondaryColour.x, ImGuiColorEditFlags_NoInputs); };
		}

		auto BrushWidthSettingsUi()
		{
			return [this]() { ImGui::SliderFloat("Width", &m_ToolWidth, 1, 256); };
		}

		auto BrushShapeSettingsUi()
		{
			return [this]() { EnumSelection("Brush Shape", m_ToolShape, BrushShapeList); };
		}

		auto AntialiasingSettingsUi()
		{
			return [this]() { ImGui::Checkbox("Antialiasing", &m_Antialiased); };
		}

		template<ToolSettingsEnum Setting>
		auto GetUiLambda()
		{
			if constexpr (Setting == ToolSettingsEnum::PrimaryColour) return PrimaryColourSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::SecondaryColour) return SecondaryColourSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::Width) return BrushWidthSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::Shape) return BrushShapeSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::Antialiasing) return AntialiasingSettingsUi();
			else static_assert(dependent_false<Setting>, "GetUiLambda: Unhandled ToolSettingsEnum value");
		}
	private:
		glm::vec4 m_PrimaryColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 m_SecondaryColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		float m_ToolWidth = 3.0f;
		BrushShapeEnum m_ToolShape = BrushShapeEnum::Circle;

		bool m_Antialiased = true;
	};

}
