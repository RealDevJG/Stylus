#include "ToolManager.h"

#include "../Tools/Options/ToolOptionsRegistry.h"
#include "../Vulkan/ComputePipeline.h"

namespace Stylus {

    ToolManager::ToolManager(std::shared_ptr<ShaderRegistry> shaderRegistry, std::shared_ptr<ToolOptionsRegistry> optionsRegistry)
        : m_ShaderRegistry(shaderRegistry),
          m_OptionsRegistry(optionsRegistry)
    {}

    void ToolManager::Use(glm::vec2 mousePos, glm::vec2 prevMousePos, ImGuiMouseButton mouseButton)
    {
        ToolOptionsRegistry::ToolHooks hooks = m_OptionsRegistry->GetHooks(m_CurrentTool);

        // TODO: change shader variables to be different left vs right click
        if (mouseButton == ImGuiMouseButton_Left)
        {
            auto shader = m_ShaderRegistry->Get(m_CurrentTool).get();
            hooks.LeftClickDispatch(shader, mousePos, prevMousePos);
        }
        else if (mouseButton == ImGuiMouseButton_Right)
        {
            auto shader = m_ShaderRegistry->Get(m_CurrentTool).get();
            hooks.RightClickDispatch(shader, mousePos, prevMousePos);
        }
    }

    void ToolManager::SetTool(ToolEnum tool)
    {
        m_CurrentTool = tool;
    }

}
