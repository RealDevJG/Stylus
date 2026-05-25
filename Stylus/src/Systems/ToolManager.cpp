#include "ToolManager.h"

namespace Stylus {

    ToolManager::ToolManager(ToolActionExecutor& toolExecutor, IToolRegistryReadonly& toolRegistryReadonly)
        : m_ToolExecutor(toolExecutor), m_ToolRegistryReadonly(toolRegistryReadonly) {}

    bool ToolManager::TryUseClickAction(bool mouseLeftDown, bool mouseRightDown, const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        if (mouseLeftDown)
        {
            return UseLeftClickAction(mousePos, prevMousePos);
        }
        else if (mouseRightDown)
        {
            return UseRightClickAction(mousePos, prevMousePos);
        }

        return false;
    }

    bool ToolManager::UseLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        ToolAction action = m_CurrentTool->GetLeftClickAction(mousePos, prevMousePos);
        return m_ToolExecutor.Execute(action);
    }

    bool ToolManager::UseRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
    {
        ToolAction action = m_CurrentTool->GetRightClickAction(mousePos, prevMousePos);
        return m_ToolExecutor.Execute(action);
    }

    void ToolManager::SetTool(ToolEnum tool)
    {
        m_CurrentToolEnum = tool;
        m_CurrentTool = &m_ToolRegistryReadonly.GetTool(tool);
    }

    const Tool* ToolManager::GetCurrentTool() const
    {
        return m_CurrentTool;
    }

}
