#include "ShaderRegistry.h"

#include "../Tools/Options/Fill/FillCanvasData.h"

#include "../Tools/EffectEnum.h"
#include "../Tools/ToolEnum.h"

namespace Stylus {

    ShaderRegistry::ShaderRegistry(std::shared_ptr<ToolStore> toolStore)
        : m_ToolStore(toolStore) {}

    void ShaderRegistry::SetCanvasImage(std::shared_ptr<Walnut::Image> canvasImage)
    {
        const std::unordered_map<Stylus::ToolEnum, Stylus::ToolData>& tools = m_ToolStore->GetTools();

        for (auto& [toolEnum, toolData] : tools)
        {
            m_ToolShaders.insert_or_assign(toolEnum, std::make_shared<ComputePipeline>(canvasImage, toolData.ShaderPath, toolData.PushConstantStructSize));
        }

        // TODO: give effects the same treatment as tools above
        m_EffectShaders.insert_or_assign(EffectEnum::FillCanvas, std::make_shared<ComputePipeline>(canvasImage, "assets/shaders/fill-canvas.spv", sizeof(FillCanvasPushData)));
    }

    std::shared_ptr<ComputePipeline> ShaderRegistry::Get(ToolEnum tool)
    {
        return m_ToolShaders[tool];
    }

    std::shared_ptr<ComputePipeline> ShaderRegistry::Get(EffectEnum effect)
    {
        return m_EffectShaders[effect];
    }

}
