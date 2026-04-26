#pragma once

#include "../Tools/ToolEnum.h"

#include <glm/glm.hpp>
#include <memory>

namespace Stylus {

	class Tool;
	class ToolRegistry;

	class ToolManager
	{
	public:
		ToolManager() = default;
		~ToolManager() = default;

		ToolManager(const ToolManager&) = delete;
		ToolManager& operator=(const ToolManager&) = delete;
		ToolManager(ToolManager&&) = delete;
		ToolManager& operator=(ToolManager&&) = delete;

		void Init(std::shared_ptr<ToolRegistry> toolRegistry);

		[[nodiscard]] bool UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const;
		[[nodiscard]] bool UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const;

		const void SetTool(ToolEnum tool);
		std::weak_ptr<const Tool> GetTool() const { return m_CurrentTool; }
	private:
		std::weak_ptr<const Tool> m_CurrentTool;
		ToolEnum m_CurrentToolEnum = ToolEnum::Brush;

		std::shared_ptr<ToolRegistry> m_ToolRegistry;
	};

}
