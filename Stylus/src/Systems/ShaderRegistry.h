#pragma once

#include "../Tools/EffectEnum.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"
#include "../Vulkan/ComputeShader.h"

#include <Walnut/Image.h>
#include <unordered_map>
#include <memory>

namespace Stylus {

	class ShaderRegistry
	{
	public:
		ShaderRegistry();
		~ShaderRegistry() = default;

		ShaderRegistry(const ShaderRegistry&) = delete;
		ShaderRegistry& operator=(const ShaderRegistry&) = delete;
		ShaderRegistry(ShaderRegistry&&) = delete;
		ShaderRegistry& operator=(ShaderRegistry&&) = delete;

		void SetCanvasImage(Walnut::Image* canvasImage) const;
		const ComputeShader* RegisterAndGet(const ToolEnum tool, const ToolData& toolData);

		[[nodiscard]] const ComputeShader* Get(const ToolEnum tool) const;
		[[nodiscard]] const ComputeShader* Get(const EffectEnum effect) const;
	private:
		std::unordered_map<ToolEnum, std::unique_ptr<ComputeShader>> m_ToolShaders{};
		std::unordered_map<EffectEnum, std::unique_ptr<ComputeShader>> m_EffectShaders{};
	};

}
