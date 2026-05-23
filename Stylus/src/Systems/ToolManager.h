#pragma once

#include "../Systems/ToolRegistry.h"
#include "../Tools/Tool.h"
#include "../Tools/ToolEnum.h"
#include "../Tools/ToolActionExecutor.h"

#include "IToolManagerState.h"
#include "IToolManagerInteractions.h"
#include "IToolRegistryReadonly.h"

#include <glm/vec2.hpp>

namespace Stylus {

	class ToolManager final : public IToolManagerState, public IToolManagerInteractions
	{
	public:
		ToolManager(ToolActionExecutor& toolExecutor, IToolRegistryReadonly& toolRegistryReadonly);
		~ToolManager() = default;

		ToolManager(const ToolManager&) = delete;
		ToolManager& operator=(const ToolManager&) = delete;
		ToolManager(ToolManager&&) = delete;
		ToolManager& operator=(ToolManager&&) = delete;

		// Returns true if the canvas was updated and should be saved to the history manager
		[[nodiscard]] bool TryUseClickAction(bool mouseLeftDown, bool mouseRightDown, const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		[[nodiscard]] bool UseLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		[[nodiscard]] bool UseRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;

		void SetTool(ToolEnum tool) override;
		[[nodiscard]] const Tool* GetCurrentTool() const override;
	private:
		ToolActionExecutor& m_ToolExecutor;
		IToolRegistryReadonly& m_ToolRegistryReadonly;

		const Tool* m_CurrentTool = nullptr;
		ToolEnum m_CurrentToolEnum = ToolEnum::None;
	};

}
