#include "ToolSettingsController.h"

#include <glm/glm.hpp>

namespace Stylus {

    ToolSettingsController::ToolSettingsController(ToolSettingsStore& settingsStore)
        : m_ToolSettingsStore(settingsStore) {}

    bool ToolSettingsController::OnKeyPressed(Walnut::KeyCode keyCode) const
    {
        if (keyCode == Walnut::KeyCode::LeftBracket || keyCode == Walnut::KeyCode::RightBracket)
        {
            float moveBy = static_cast<float>(keyCode) - 92.0f;
            ChangeBrushWidthBy(moveBy);
            return true;
        }

        return false;
    }

    float ToolSettingsController::ChangeBrushWidthBy(float moveBy) const
    {
        auto& value = m_ToolSettingsStore.GetValue<TSE::Width>();
        value = glm::floor(glm::clamp(value + moveBy, 1.0f, 256.0f));
        return value;
    }

}
