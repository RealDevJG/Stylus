#pragma once

#include "ComputeShader.h"
#include "../Tools/EffectEnum.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include <Walnut/Image.h>
#include <unordered_map>

namespace Stylus {

	class ShaderRegistry
	{
	public:
		ShaderRegistry() = default;
		~ShaderRegistry() = default;

		ShaderRegistry(const ShaderRegistry&) = delete;
		ShaderRegistry& operator=(const ShaderRegistry&) = delete;
		ShaderRegistry(ShaderRegistry&&) = delete;
		ShaderRegistry& operator=(ShaderRegistry&&) = delete;

		void Init();
		void SetCanvasImage(std::shared_ptr<Walnut::Image> canvasImage);

		std::shared_ptr<ComputeShader> RegisterAndGet(ToolEnum tool, const ToolData& toolData);
		[[nodiscard]] std::shared_ptr<ComputeShader> Get(ToolEnum tool) const;
		[[nodiscard]] std::shared_ptr<ComputeShader> Get(EffectEnum effect) const;

		void Cleanup();
	private:
		std::unordered_map<ToolEnum, std::shared_ptr<ComputeShader>> m_ToolShaders{};
		std::unordered_map<EffectEnum, std::shared_ptr<ComputeShader>> m_EffectShaders{};
	};

}
