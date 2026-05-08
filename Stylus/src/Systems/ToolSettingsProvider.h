#pragma once

#include "../Tools/ToolSettingsEnum.h"
#include "ToolSettingsRegistry.h"

namespace Stylus {

	template<ToolSettingsEnum... AllowedSettings>
	class ToolSettingsProvider
	{
	public:
		ToolSettingsProvider(ToolSettingsRegistry* toolSettingsRegistry)
			: m_ToolSettingsRegistry(toolSettingsRegistry) {}

		template<ToolSettingsEnum Setting>
		[[nodiscard]] constexpr const auto& Get() const
		{
			static_assert(((Setting == AllowedSettings) || ...), "Get: Tool is trying to access a setting it isn't configured to. It needs adding to ToolRegistry.cpp");
			return m_ToolSettingsRegistry->GetValue<Setting>();
		}

		template<ToolSettingsEnum Setting>
		constexpr void Set(auto newValue) const
		{
			static_assert(((Setting == AllowedSettings) || ...), "Get: Tool is trying to access a setting it isn't configured to. It needs adding to ToolRegistry.cpp");

			auto& value = m_ToolSettingsRegistry->GetValue<Setting>();
			value = newValue;
		}
	private:
		ToolSettingsRegistry* m_ToolSettingsRegistry;
	};

}
