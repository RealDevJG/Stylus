#include "ToolSettingsController.h"

#include "ToolSettingsRegistry.h"
#include <glm/glm.hpp>

namespace Stylus {

    ToolSettingsController::ToolSettingsController(std::shared_ptr<ToolSettingsRegistry> settingsRegistry)
        : m_ToolSettingsRegistry(settingsRegistry) {}

    float ToolSettingsController::ChangeBrushWidthBy(float moveBy) const
    {
        auto& value = m_ToolSettingsRegistry->GetValue<TSE::Width>();
        value = glm::floor(glm::clamp(value + moveBy, 1.0f, 256.0f));
        return value;
    }

}
