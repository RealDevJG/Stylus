#include "ShaderRegistry.h"

#include "../Tools/Fill/FillCanvasData.h"
#include "../Tools/ToolRegistry.h"

namespace Stylus {

    void ShaderRegistry::Init()
    {
        // TODO: register effects externally like tools
        m_EffectShaders.insert_or_assign(EffectEnum::FillCanvas, std::make_shared<ComputeShader>("assets/shaders/fill-canvas.spv", sizeof(FillCanvasPushData)));
    }

    void ShaderRegistry::SetCanvasImage(std::shared_ptr<Walnut::Image> canvasImage)
    {
        for (auto& [toolEnum, shader] : m_ToolShaders)
        {
            shader->SetImage(canvasImage);
        }

        for (auto& [toolEnum, shader] : m_EffectShaders)
        {
            shader->SetImage(canvasImage);
        }
    }

    // TODO: refactor the entire system of registering shaders and tools so they can work independently
    std::shared_ptr<ComputeShader> ShaderRegistry::RegisterAndGet(ToolEnum tool, const ToolData& toolData)
    {
        std::shared_ptr<ComputeShader> shader = m_ToolShaders[tool];

        if (shader)
        {
            return shader;
        }

        m_ToolShaders.insert_or_assign(tool, std::make_shared<ComputeShader>(toolData.ShaderPath, toolData.PushConstantStructSize));

        return Get(tool);
    }

    std::shared_ptr<ComputeShader> ShaderRegistry::Get(ToolEnum tool) const
    {
        return m_ToolShaders.at(tool);
    }

    std::shared_ptr<ComputeShader> ShaderRegistry::Get(EffectEnum effect) const
    {
        return m_EffectShaders.at(effect);
    }

    void ShaderRegistry::Cleanup()
    {
        m_ToolShaders.clear();
        m_EffectShaders.clear();
    }

}
