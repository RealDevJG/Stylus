#include "ShaderRegistry.h"

#include "../Tools/Fill/FillCanvasData.h"
#include "../Systems/ToolRegistry.h"

namespace Stylus {

    ShaderRegistry::ShaderRegistry()
    {
        // TODO: when there are more effects, register them externally like tools
        m_EffectShaders.try_emplace(
            EffectEnum::FillCanvas,
            std::make_unique<ComputeShader>(
                "assets/shaders/fill-canvas.spv",
                static_cast<uint32_t>(sizeof(FillCanvasPushData))
            )
        );
    }

    void ShaderRegistry::SetCanvasImage(Walnut::Image* canvasImage) const
    {
        for (const auto& [toolEnum, shader] : m_ToolShaders)
        {
            shader->SetImage(canvasImage);
        }

        for (const auto& [toolEnum, shader] : m_EffectShaders)
        {
            shader->SetImage(canvasImage);
        }
    }

    const ComputeShader* ShaderRegistry::RegisterAndGet(const ToolEnum toolEnum, const ToolData& toolData)
    {
        auto [it, emplaced] = m_ToolShaders.try_emplace(toolEnum, std::make_unique<ComputeShader>(toolData.ShaderPath, toolData.PushConstantStructSize));
        return it->second.get();
    }

    const ComputeShader* ShaderRegistry::Get(const ToolEnum toolEnum) const
    {
        auto it = m_ToolShaders.find(toolEnum);
        assert(it != m_ToolShaders.end() && "Couldn't find tool shader in ShaderRegistry::Get");
        return it->second.get();
    }

    const ComputeShader* ShaderRegistry::Get(const EffectEnum effectEnum) const
    {
        auto it = m_EffectShaders.find(effectEnum);
        assert(it != m_EffectShaders.end() && "Couldn't find effect shader in ShaderRegistry::Get");
        return it->second.get();
    }

}
