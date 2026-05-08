#pragma once

#include "../Tools/Brush/BrushShape.h"
#include "../Tools/ToolSettingsEnum.h"
#include "../Ui/EnumSelection.h"
#include "../Ui/DrawUiRow.h"

#include <imgui.h>
#include <glm/glm.hpp>

namespace Stylus {

	class ToolSettingsRegistry
	{
	public:
		ToolSettingsRegistry() = default;
		~ToolSettingsRegistry() = default;

		ToolSettingsRegistry(const ToolSettingsRegistry&) = delete;
		ToolSettingsRegistry& operator=(const ToolSettingsRegistry&) = delete;
		ToolSettingsRegistry(ToolSettingsRegistry&&) = delete;
		ToolSettingsRegistry& operator=(ToolSettingsRegistry&&) = delete;

		template<ToolSettingsEnum Setting>
		constexpr auto& GetValue()
		{
			return GetValueImpl<Setting>(*this);
		}

		template<ToolSettingsEnum Setting>
		constexpr auto& GetValue() const
		{
			return GetValueImpl<Setting>(*this);
		}

		template<ToolSettingsEnum... Settings>
		constexpr auto CreateUiDrawer()
		{
			return [this]() {
				UI::DrawToolbarRow(this->GetUiLambda<Settings>()...);
			};
		}
	private:
		template<ToolSettingsEnum Setting, typename Self>
		static constexpr auto& GetValueImpl(Self& self)
		{
			if constexpr (Setting == ToolSettingsEnum::PrimaryColour) return self.m_PrimaryColour;
			else if constexpr (Setting == ToolSettingsEnum::SecondaryColour) return self.m_SecondaryColour;
			else if constexpr (Setting == ToolSettingsEnum::Width) return self.m_ToolWidth;
			else if constexpr (Setting == ToolSettingsEnum::Shape) return self.m_ToolShape;
			else if constexpr (Setting == ToolSettingsEnum::Antialiasing) return self.m_Antialiased;
			else static_assert(false, "GetValue: Unhandled ToolSettingEnum value");
		}

		constexpr auto PrimaryColourSettingsUi()
		{
			return [this]() { ImGui::ColorEdit4("Primary Colour", &m_PrimaryColour.x, ImGuiColorEditFlags_NoInputs); };
		}

		constexpr auto SecondaryColourSettingsUi()
		{
			return [this]() { ImGui::ColorEdit4("Secondary Colour", &m_SecondaryColour.x, ImGuiColorEditFlags_NoInputs); };
		}

		constexpr auto BrushWidthSettingsUi()
		{
			return [this]() { ImGui::SliderFloat("Width", &m_ToolWidth, 1, 256); };
		}

		constexpr auto BrushShapeSettingsUi()
		{
			return [this]() { UI::EnumSelection("Brush Shape", m_ToolShape, std::span{ BrushShapeList }); };
		}

		constexpr auto AntialiasingSettingsUi()
		{
			return [this]() { ImGui::Checkbox("Antialiasing", &m_Antialiased); };
		}

		template<ToolSettingsEnum Setting>
		constexpr auto GetUiLambda()
		{
			if constexpr (Setting == ToolSettingsEnum::PrimaryColour) return PrimaryColourSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::SecondaryColour) return SecondaryColourSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::Width) return BrushWidthSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::Shape) return BrushShapeSettingsUi();
			else if constexpr (Setting == ToolSettingsEnum::Antialiasing) return AntialiasingSettingsUi();
			else static_assert(false, "GetUiLambda: Unhandled ToolSettingsEnum value");
		}
	private:
		glm::vec4 m_PrimaryColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 m_SecondaryColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		float m_ToolWidth = 3.0f;
		BrushShapeEnum m_ToolShape = BrushShapeEnum::Circle;

		bool m_Antialiased = true;
	};

}
