#pragma once

#include <memory>

namespace Stylus {

	class ToolSettingsRegistry;

	class ToolSettingsController
	{
	public:
		ToolSettingsController(std::shared_ptr<ToolSettingsRegistry> settingsRegistry);
		~ToolSettingsController() = default;

		ToolSettingsController(const ToolSettingsController&) = delete;
		ToolSettingsController& operator=(const ToolSettingsController&) = delete;
		ToolSettingsController(ToolSettingsController&&) = delete;
		ToolSettingsController& operator=(ToolSettingsController&&) = delete;

		float ChangeBrushWidthBy(float moveBy) const;
	private:
		std::shared_ptr<ToolSettingsRegistry> m_ToolSettingsRegistry;
	};

}
