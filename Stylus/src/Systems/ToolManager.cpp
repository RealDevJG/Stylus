#include "ToolManager.h"

#include "../Tools/Tool.h"
#include "../Systems/ToolRegistry.h"

namespace Stylus {

    ToolManager::ToolManager(std::shared_ptr<ToolRegistry> toolRegistry)
    {
        m_ToolRegistry = toolRegistry;
        SetTool(ToolEnum::Brush);
    }

    bool ToolManager::UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        if (m_CurrentTool)
        {
            return m_CurrentTool->UseLeftClick(mousePos, prevMousePos);
        }

        return false;
    }

    bool ToolManager::UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        if (m_CurrentTool)
        {
            return m_CurrentTool->UseRightClick(mousePos, prevMousePos);
        }

        return false;
    }

    const void ToolManager::SetTool(ToolEnum tool)
    {
        m_CurrentToolEnum = tool;
        m_CurrentTool = m_ToolRegistry->GetTool(tool);
    }

}
