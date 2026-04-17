#pragma once

#include "ToolSettingsEnum.h"
#include "ToolOptionsRegistry.h"

namespace Stylus {

	template<ToolSettingsEnum... AllowedSettings>
	class ToolSettingsProvider
	{
	public:
		ToolSettingsProvider(ToolOptionsRegistry* optionsRegistry)
			: m_OptionsRegistry(optionsRegistry) {}

		template<ToolSettingsEnum Setting>
		const auto& Get() const
		{
			static_assert(((Setting == AllowedSettings) || ...), "Get: Tool is trying to access a setting it isn't configured to. It needs adding to ToolStore.cpp");
			return m_OptionsRegistry->GetValue<Setting>();
		}
	private:
		ToolOptionsRegistry* m_OptionsRegistry;
	};

}
