#pragma once

#include "Brush/BrushShape.h"
#include "../ToolEnum.h"
#include "../../Vulkan/ComputePipeline.h"

#include <glm/glm.hpp>

#include <functional>
#include <unordered_map>
#include <vector>

namespace Stylus {

	class ToolOptionsRegistry
	{
	public:
		using DispatchDefinition = void(ComputePipeline* shader, glm::vec2 mousePos, glm::vec2 prevMousePos);

		struct ToolHooks
		{
			std::function<void()> DrawUI;
			std::function<DispatchDefinition> LeftClickDispatch;
			std::function<DispatchDefinition> RightClickDispatch;
		};

		ToolOptionsRegistry();
		[[nodiscard]] ToolHooks GetHooks(ToolEnum tool) { return m_ToolMap.at(tool); }
		[[nodiscard]] std::vector<ToolEnum> GetTools();
	private:
		glm::vec4 m_PrimaryColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 m_SecondaryColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		float m_BrushWidth = 3.0f;
		BrushShapeEnum m_BrushShape = BrushShapeEnum::Circle;

		bool m_Antialiased = true;

		std::unordered_map<ToolEnum, ToolHooks> m_ToolMap;
	};

}
