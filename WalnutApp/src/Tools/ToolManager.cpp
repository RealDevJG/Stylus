#include "ToolManager.h"

#include "../ToolOptions/ToolOptionsRegistry.h"
#include "../Vulkan/ComputePipeline.h"

namespace Stylus {

    ToolManager::ToolManager(std::shared_ptr<ShaderRegistry> shaderRegistry, std::shared_ptr<ToolOptionsRegistry> optionsRegistry)
        : m_ShaderRegistry(shaderRegistry),
          m_OptionsRegistry(optionsRegistry)
    {}

    void ToolManager::Use(glm::vec2 mousePos)
    {
        ToolOptionsRegistry::ToolHooks hooks = m_OptionsRegistry->GetHooks(m_CurrentTool);
        hooks.Dispatch(m_ShaderRegistry->Get(m_CurrentTool).get(), mousePos);
    }

    void ToolManager::SetTool(ToolEnum tool)
    {
        m_CurrentTool = tool;
    }

}
