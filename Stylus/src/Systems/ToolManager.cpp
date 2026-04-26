#include "ToolManager.h"

#include "../Tools/Tool.h"
#include "../Systems/ToolRegistry.h"

namespace Stylus {

    void ToolManager::Init(std::shared_ptr<ToolRegistry> toolRegistry)
    {
        m_ToolRegistry = toolRegistry;
    }

    bool ToolManager::UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
    {
        if (auto tool = m_CurrentTool.lock())
        {
            return tool->UseLeftClick(mousePos, prevMousePos);
        }

        return false;
    }

    bool ToolManager::UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
    {
        if (auto tool = m_CurrentTool.lock())
        {
            return tool->UseRightClick(mousePos, prevMousePos);
        }

        return false;
    }

    const void ToolManager::SetTool(ToolEnum tool)
    {
        m_CurrentToolEnum = tool;
        m_CurrentTool = m_ToolRegistry->GetTool(tool);
    }

}
