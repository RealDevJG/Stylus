#pragma once

namespace Stylus {

	class ToolSettingsStore;

	class IToolSettingsUpdater
	{
	public:
		virtual ~IToolSettingsUpdater() = default;
		virtual void Apply(ToolSettingsStore& settingsStore) = 0;
	};

}
