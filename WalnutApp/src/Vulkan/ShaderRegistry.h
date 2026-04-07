#pragma once

#include "../Tools/ToolEnum.h"
#include "../Tools/EffectEnum.h"
#include "../Vulkan/ComputePipeline.h"

#include <Walnut/Image.h>

#include <array>
#include <unordered_map>

namespace Stylus {

	class ShaderRegistry
	{
	public:
		void SetCanvasImage(std::shared_ptr<Walnut::Image> canvasImage);

		[[nodiscard]] std::shared_ptr<ComputePipeline> Get(ToolEnum tool);
		[[nodiscard]] std::shared_ptr<ComputePipeline> Get(EffectEnum effect);
	private:
		std::unordered_map<ToolEnum, std::shared_ptr<ComputePipeline>> m_ToolShaders{};
		std::unordered_map<EffectEnum, std::shared_ptr<ComputePipeline>> m_EffectShaders{};
	};

}
