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
		ToolManager(std::shared_ptr<ToolRegistry> toolRegistry);
		~ToolManager() = default;

		ToolManager(const ToolManager&) = delete;
		ToolManager& operator=(const ToolManager&) = delete;
		ToolManager(ToolManager&&) = delete;
		ToolManager& operator=(ToolManager&&) = delete;

		[[nodiscard]] bool UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const;
		[[nodiscard]] bool UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const;

		const void SetTool(ToolEnum tool);
		[[nodiscard]] constexpr const Tool* GetTool() const { return m_CurrentTool; }
	private:
		const Tool* m_CurrentTool;
		ToolEnum m_CurrentToolEnum = ToolEnum::Brush;

		std::shared_ptr<ToolRegistry> m_ToolRegistry;
	};

}
