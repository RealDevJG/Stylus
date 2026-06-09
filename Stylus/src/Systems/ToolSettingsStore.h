#pragma once

#include "../Tools/Brush/BrushShape.h"
#include "../Tools/ToolSettingsEnum.h"

#include <glm/vec4.hpp>

namespace Stylus {

	class ToolSettingsStore
	{
	public:
		ToolSettingsStore() = default;
		~ToolSettingsStore() = default;

		ToolSettingsStore(const ToolSettingsStore&) = delete;
		ToolSettingsStore& operator=(const ToolSettingsStore&) = delete;
		ToolSettingsStore(ToolSettingsStore&&) = delete;
		ToolSettingsStore& operator=(ToolSettingsStore&&) = delete;

		template<ToolSettingsEnum Setting>
		constexpr auto& GetValue(this auto& self)
		{
			if constexpr (Setting == ToolSettingsEnum::PrimaryColour) return self.m_PrimaryColour;
			else if constexpr (Setting == ToolSettingsEnum::SecondaryColour) return self.m_SecondaryColour;
			else if constexpr (Setting == ToolSettingsEnum::Width) return self.m_ToolWidth;
			else if constexpr (Setting == ToolSettingsEnum::Shape) return self.m_ToolShape;
			else if constexpr (Setting == ToolSettingsEnum::Antialiasing) return self.m_Antialiased;
			else static_assert(false, "GetValue: Unhandled ToolSettingEnum value");
		}
	private:
		glm::vec4 m_PrimaryColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 m_SecondaryColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		float m_ToolWidth = 3.0f;
		BrushShapeEnum m_ToolShape = BrushShapeEnum::Circle;

		bool m_Antialiased = false;
	};

}
