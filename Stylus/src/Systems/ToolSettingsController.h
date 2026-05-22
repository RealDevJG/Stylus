#pragma once

#include "ToolSettingsStore.h"
#include <Walnut/Input/KeyCodes.h>

namespace Stylus {

	class ToolSettingsController
	{
	public:
		ToolSettingsController(ToolSettingsStore& settingsStore);
		~ToolSettingsController() = default;

		ToolSettingsController(const ToolSettingsController&) = delete;
		ToolSettingsController& operator=(const ToolSettingsController&) = delete;
		ToolSettingsController(ToolSettingsController&&) = delete;
		ToolSettingsController& operator=(ToolSettingsController&&) = delete;

		bool OnKeyPressed(Walnut::KeyCode keyCode) const;
	private:
		float ChangeBrushWidthBy(float moveBy) const;
	private:
		ToolSettingsStore& m_ToolSettingsStore;
	};

}
