#include "ToolManager.h"

#include "Tool.h"
#include "../Tools/ToolRegistry.h"

namespace Stylus {

    void ToolManager::Init(std::shared_ptr<ToolRegistry> toolRegistry)
    {
        m_ToolRegistry = toolRegistry;
    }

    void ToolManager::UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
    {
        if (auto tool = m_CurrentTool.lock())
        {
            tool->UseLeftClick(mousePos, prevMousePos);
        }
    }

    void ToolManager::UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
    {
        if (auto tool = m_CurrentTool.lock())
        {
            tool->UseRightClick(mousePos, prevMousePos);
        }
    }

    const void ToolManager::SetTool(ToolEnum tool)
    {
        m_CurrentToolEnum = tool;
        m_CurrentTool = m_ToolRegistry->GetTool(tool);
    }

}
