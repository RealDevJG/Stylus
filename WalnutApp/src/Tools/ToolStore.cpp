#include "ToolStore.h"

#include "Options/Brush/BrushPushData.h"
#include "Options/Eraser/EraserPushData.h"

namespace Stylus {

	ToolStore::ToolStore()
	{
		m_ToolData.insert_or_assign(ToolEnum::Brush, ToolData{ "Brush", "assets/shaders/brush.spv", sizeof(BrushPushData) });
		m_ToolData.insert_or_assign(ToolEnum::Eraser, ToolData{ "Eraser", "assets/shaders/brush.spv", sizeof(EraserPushData) });
	}

	const std::unordered_map<Stylus::ToolEnum, Stylus::ToolData>& ToolStore::GetTools()
	{
		return m_ToolData;
	}

	ToolData ToolStore::GetToolData(ToolEnum tool)
	{
		return m_ToolData[tool];
	}

}
