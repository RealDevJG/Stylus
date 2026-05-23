#include "ColourPickerTool.h"

#include "../../Systems/ToolSettingsUpdater.h"
#include "../../Utils/CanvasUtils.h"
#include "../../Vulkan/PushData.h"
#include "../ToolSettingsEnum.h"

#include <memory>

namespace Stylus {

	ColourPickerTool::ColourPickerTool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy, ToolSettingsStore& settingsStore)
		: Tool(toolData, drawSettingsUIStrategy), m_SettingsStore(settingsStore) {}

    ToolAction ColourPickerTool::GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        glm::vec4 newColour = Utils::GetColourAt(mousePos);

        return ToolAction{
            ToolActionType::UpdateSettingValueAction,
            UpdateSettingValueAction{
                .SettingsUpdater = std::make_unique<ToolSettingsUpdater<TSE::PrimaryColour, glm::vec4>>(newColour)
            }
        };
    }

	ToolAction ColourPickerTool::GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
        glm::vec4 newColour = Utils::GetColourAt(mousePos);

        return ToolAction{
            ToolActionType::UpdateSettingValueAction,
            UpdateSettingValueAction{
                .SettingsUpdater = std::make_unique<ToolSettingsUpdater<TSE::SecondaryColour, glm::vec4>>(newColour)
            }
        };
    }

    ToolAction ColourPickerTool::DrawOverlayHint(const glm::vec2 mousePos, float scale) const
    {
        BrushLikeOverlayPushData pushData{
            .MousePos = mousePos,
            .Width = 1.0f,
            .Scale = scale,
            .Shape = static_cast<int>(BrushShapeEnum::Circle)
        };

        return ToolAction{
            ToolActionType::GraphicsShaderAction,
            GraphicsShaderAction{
                .ShaderEnum = GraphicsShaderEnum::BrushLikeToolOverlay,
                .PushData = Utils::CopyToBytes(pushData),
                .PushDataSize = GetToolData().GraphicsPushConstantSize
            }
        };
    }

}
