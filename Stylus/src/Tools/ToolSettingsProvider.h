#pragma once

#include "ToolSettingsEnum.h"
#include "ToolSettingsRegistry.h"

#include <memory>

namespace Stylus {

	template<ToolSettingsEnum... AllowedSettings>
	class ToolSettingsProvider
	{
	public:
		ToolSettingsProvider(std::shared_ptr<ToolSettingsRegistry> toolSettingsRegistry)
			: m_ToolSettingsRegistry(toolSettingsRegistry) {}

		template<ToolSettingsEnum Setting>
		const auto& Get() const
		{
			static_assert(((Setting == AllowedSettings) || ...), "Get: Tool is trying to access a setting it isn't configured to. It needs adding to ToolRegistry.cpp");
			return m_ToolSettingsRegistry->GetValue<Setting>();
		}

		template<ToolSettingsEnum Setting>
		void Set(auto newVal) const
		{
			static_assert(((Setting == AllowedSettings) || ...), "Get: Tool is trying to access a setting it isn't configured to. It needs adding to ToolRegistry.cpp");

			auto& val = m_ToolSettingsRegistry->GetValue<Setting>();
			val = newVal;
		}
	private:
		std::shared_ptr<ToolSettingsRegistry> m_ToolSettingsRegistry;
	};

}
