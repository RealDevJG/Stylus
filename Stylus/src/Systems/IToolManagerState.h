#pragma once

#include "../Tools/ToolEnum.h"

namespace Stylus {

	class Tool;

	class IToolManagerState
	{
	public:
		virtual ~IToolManagerState() = default;

		virtual void SetTool(ToolEnum tool) = 0;
		[[nodiscard]] virtual const Tool* GetCurrentTool() const = 0;
	};

}
