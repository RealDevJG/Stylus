#pragma once

#include <glm/vec2.hpp>

namespace Stylus {

	// TODO: Use this after implementing central input manager
	class IToolManagerInteractions
	{
	public:
		virtual ~IToolManagerInteractions() = default;

		[[nodiscard]] virtual bool TryUseClickAction(bool mouseLeftDown, bool mouseRightDown, const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		[[nodiscard]] virtual bool UseLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
		[[nodiscard]] virtual bool UseRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const = 0;
	};

}
