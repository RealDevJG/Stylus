#pragma once

#include "IToolSettingsUpdater.h"
#include "ToolSettingsStore.h"

#include <utility>

namespace Stylus {

	template<ToolSettingsEnum SettingsEnum, typename ValueType>
	class ToolSettingsUpdater final : public IToolSettingsUpdater
	{
	public:
		explicit ToolSettingsUpdater(ValueType newValue)
			: m_NewValue(std::move(newValue)) {}

		void Apply(ToolSettingsStore& settingsStore) override
		{
			ValueType& value = settingsStore.GetValue<SettingsEnum>();
			value = m_NewValue;
		}
	private:
		ValueType m_NewValue;
	};

}
