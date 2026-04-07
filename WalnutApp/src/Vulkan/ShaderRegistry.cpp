#include "ShaderRegistry.h"

#include "../ToolOptions/Brush/BrushPushData.h"
#include "../ToolOptions/Fill/FillCanvasData.h"
#include "../Tools/EffectEnum.h"
#include "../Tools/ToolEnum.h"

namespace Stylus {

    void ShaderRegistry::SetCanvasImage(std::shared_ptr<Walnut::Image> canvasImage)
    {
        m_ToolShaders.insert_or_assign(ToolEnum::Brush, std::make_shared<ComputePipeline>(canvasImage, "assets/shaders/brush.spv", sizeof(BrushPushData)));
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
