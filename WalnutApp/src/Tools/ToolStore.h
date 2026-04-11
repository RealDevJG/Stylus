#pragma once

#include "ToolData.h"
#include "ToolEnum.h"

#include <unordered_map>
#include <vector>

namespace Stylus {

	class ToolStore
	{
	public:
		ToolStore();

		[[nodiscard]] const std::unordered_map<Stylus::ToolEnum, Stylus::ToolData>& GetTools();
		[[nodiscard]] ToolData GetToolData(ToolEnum tool);
	private:
		std::unordered_map<ToolEnum, ToolData> m_ToolData;
	};

}
