#include "ToolStore.h"

#include "Brush/BrushTool.h"
#include "Brush/BrushPushData.h"
#include "ColourPicker/ColourPickerTool.h"
#include "Eraser/EraserTool.h"
#include "Eraser/EraserPushData.h"

#include "ToolSettingsEnum.h"
#include "ToolOptionsRegistry.h"
#include "../CoreContext.h"
#include "../Vulkan/ShaderRegistry.h"

namespace Stylus {

	void ToolStore::Init()
	{
		ShaderRegistry& shaderRegistry = CoreContext::s_Instance->GetShaderRegistry();
		ToolOptionsRegistry& optionsRegistry = CoreContext::s_Instance->GetToolOptionsRegistry();

		{
			ToolData toolData{
				"Brush",
				"assets/shaders/brush.spv",
				sizeof(BrushPushData),
				Walnut::KeyCode::B
			};

			const BrushTool brushTool = BrushTool{
				optionsRegistry.CreateUiDrawer<TSE::Width, TSE::Shape, TSE::PrimaryColour, TSE::SecondaryColour, TSE::Antialiasing>(),
				toolData,
				shaderRegistry.RegisterAndGet(ToolEnum::Brush, toolData),
				BrushSettingsContext{ &optionsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::Brush);
			m_Tools.insert_or_assign(ToolEnum::Brush, std::make_shared<BrushTool>(brushTool));
		}

		{
			ToolData toolData{
				"Eraser",
				"assets/shaders/brush.spv",
				sizeof(EraserPushData),
				Walnut::KeyCode::E
			};

			const EraserTool eraserTool = EraserTool{
				optionsRegistry.CreateUiDrawer<TSE::Width, TSE::Shape, TSE::Antialiasing>(),
				toolData,
				shaderRegistry.RegisterAndGet(ToolEnum::Eraser, toolData),
				EraserSettingsContext{ &optionsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::Eraser);
			m_Tools.insert_or_assign(ToolEnum::Eraser, std::make_shared<EraserTool>(eraserTool));
		}

		{
			ToolData toolData{};
			toolData.Name = "Colour Picker";
			toolData.KeyShortcut = Walnut::KeyCode::I;

			const ColourPickerTool colourPickerTool = ColourPickerTool{
				optionsRegistry.CreateUiDrawer<TSE::PrimaryColour, TSE::SecondaryColour>(),
				toolData,
				ColourPickerSettingsContext{ &optionsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::ColourPicker);
			m_Tools.insert_or_assign(ToolEnum::ColourPicker, std::make_shared<ColourPickerTool>(colourPickerTool));
		}
	}

	std::shared_ptr<const Tool> ToolStore::GetTool(ToolEnum tool) const
	{
		return m_Tools.at(tool);
	}

	ToolEnum ToolStore::GetToolEnum(Walnut::KeyCode shortcut) const
	{
		auto it = m_KeyShortcuts.find(shortcut);

		if (it != m_KeyShortcuts.end())
			return it->second;

		return ToolEnum::None;
	}

	const std::unordered_map<Stylus::ToolEnum, std::shared_ptr<const Tool>>& ToolStore::GetTools() const
	{
		return m_Tools;
	}

}
