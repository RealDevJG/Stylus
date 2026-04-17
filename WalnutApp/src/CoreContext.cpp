#include "CoreContext.h"

namespace Stylus {

    CoreContext::CoreContext()
        : m_ToolManager(std::make_unique<ToolManager>()),
          m_OptionsRegistry(std::make_unique<ToolOptionsRegistry>()),
          m_ShaderRegistry(std::make_unique<ShaderRegistry>()),
          m_ToolStore(std::make_unique<ToolStore>())
    {
        s_Instance = this;
    }

    void CoreContext::Init()
    {
        m_ShaderRegistry->Init();
        m_ToolStore->Init();
    }

    ToolManager& CoreContext::GetToolManager()
    {
        return *m_ToolManager.get();
    }

    ToolOptionsRegistry& CoreContext::GetToolOptionsRegistry()
    {
        return *m_OptionsRegistry.get();
    }

    ToolStore& CoreContext::GetToolStore()
    {
        return *m_ToolStore.get();
    }

    ShaderRegistry& CoreContext::GetShaderRegistry()
    {
        return *m_ShaderRegistry.get();
    }

}
