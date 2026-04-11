#pragma once

#include "../Vulkan/ComputePipeline.h"
#include "../Vulkan/ShaderRegistry.h"
#include "Options/ToolOptionsRegistry.h"
#include "ToolEnum.h"

#include <imgui.h>
#include <glm/glm.hpp>

#include <functional>

namespace Stylus {

	class ToolManager
	{
	public:
		ToolManager(std::shared_ptr<ShaderRegistry> shaderRegistry, std::shared_ptr<ToolOptionsRegistry> optionsRegistry);

		void Use(glm::vec2 mousePos, glm::vec2 prevMousePos, ImGuiMouseButton mouseButton);
		void SetTool(ToolEnum tool);
		ToolEnum GetTool() const { return m_CurrentTool; }
	private:
		ToolEnum m_CurrentTool = ToolEnum::Brush;

		std::shared_ptr<ShaderRegistry> m_ShaderRegistry;
		std::shared_ptr<ToolOptionsRegistry> m_OptionsRegistry;
	};

}
