#pragma once

#include "../Tools/Tool.h"
#include "../Tools/ToolEnum.h"

#include <Walnut/Input/KeyCodes.h>
#include <memory>

namespace Stylus {

	class IToolRegistryReadonly
	{
	public:
		virtual ~IToolRegistryReadonly() = default;

		[[nodiscard]] virtual const Tool& GetTool(ToolEnum toolEnum) const = 0;
		[[nodiscard]] virtual ToolEnum GetToolEnum(Walnut::KeyCode shortcut) const = 0;
		[[nodiscard]] virtual const std::unordered_map<ToolEnum, std::unique_ptr<const Tool>>& GetTools() const = 0;
	};

}
