#pragma once

#include "../Vulkan/ComputePipeline.h"
#include "../Vulkan/ShaderRegistry.h"
#include "../ToolOptions/ToolOptionsRegistry.h"
#include "ToolEnum.h"

#include <functional>
#include <glm/glm.hpp>

namespace Stylus {

	class ToolManager
	{
	public:
		// NOTE: should registries be shared_ptr, raw ptr or globally accessible
		ToolManager(std::shared_ptr<ShaderRegistry> shaderRegistry, std::shared_ptr<ToolOptionsRegistry> optionsRegistry);

		void Use(glm::vec2 mousePos);
		void SetTool(ToolEnum tool);
		ToolEnum GetTool() const { return m_CurrentTool; }
	private:
		ToolEnum m_CurrentTool = ToolEnum::Brush;

		std::shared_ptr<ComputePipeline> m_ToolShader;
		std::shared_ptr<ShaderRegistry> m_ShaderRegistry;
		std::shared_ptr<ToolOptionsRegistry> m_OptionsRegistry;

		std::function<void(ComputePipeline* shader, glm::vec2 mousePos)> m_DispatchShader;
	};

}
