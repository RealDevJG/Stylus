#include "ToolManager.h"

#include "Tool.h"
#include "../CoreContext.h"
#include "../Tools/ToolStore.h"

namespace Stylus {

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

        const ToolStore& toolStore = CoreContext::s_Instance->GetToolStore();
        m_CurrentTool = toolStore.GetTool(tool);
    }

}
