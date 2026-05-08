#include "ToolRegistry.h"

#include "../Tools/Brush/BrushTool.h"
#include "../Tools/Brush/BrushPushData.h"
#include "../Tools/ColourPicker/ColourPickerTool.h"
#include "../Tools/Eraser/EraserTool.h"
#include "../Tools/Eraser/EraserPushData.h"
#include "../Tools/ToolSettingsEnum.h"

#include "ToolSettingsRegistry.h"
#include "ShaderRegistry.h"

namespace Stylus {

	ToolRegistry::ToolRegistry(std::shared_ptr<ToolSettingsRegistry> toolSettingsRegistry, std::shared_ptr<ShaderRegistry> shaderRegistry)
	{
		RegisterTools(toolSettingsRegistry.get(), shaderRegistry.get());
	}

	const Tool* ToolRegistry::GetTool(ToolEnum toolEnum) const
	{
		auto it = m_Tools.find(toolEnum);
		assert(it != m_Tools.end() && "Tool not found in ToolRegistry::GetTool");
		return it->second.get();
	}

	ToolEnum ToolRegistry::GetToolEnum(Walnut::KeyCode shortcut) const
	{
		if (auto it = m_KeyShortcuts.find(shortcut); it != m_KeyShortcuts.end())
		{
			return it->second;
		}

		return ToolEnum::None;
	}

	const std::unordered_map<Stylus::ToolEnum, std::unique_ptr<const Tool>>& ToolRegistry::GetTools() const
	{
		return m_Tools;
	}

	void ToolRegistry::RegisterTools(ToolSettingsRegistry* toolSettingsRegistry, ShaderRegistry* shaderRegistry)
	{
		{ // BRUSH TOOL
			ToolData toolData{
				"Brush",
				"assets/shaders/brush.spv",
				sizeof(BrushPushData),
				Walnut::KeyCode::B
			};

			const BrushTool brushTool = BrushTool{
				toolSettingsRegistry->CreateUiDrawer<TSE::Width, TSE::Shape, TSE::PrimaryColour, TSE::SecondaryColour, TSE::Antialiasing>(),
				toolData,
				shaderRegistry->RegisterAndGet(ToolEnum::Brush, toolData),
				BrushSettingsContext{ toolSettingsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::Brush);
			m_Tools.insert_or_assign(ToolEnum::Brush, std::make_unique<BrushTool>(brushTool));
		}

		{ // ERASER TOOL
			ToolData toolData{
				"Eraser",
				"assets/shaders/brush.spv",
				sizeof(EraserPushData),
				Walnut::KeyCode::E
			};

			const EraserTool eraserTool = EraserTool{
				toolSettingsRegistry->CreateUiDrawer<TSE::Width, TSE::Shape, TSE::Antialiasing>(),
				toolData,
				shaderRegistry->RegisterAndGet(ToolEnum::Eraser, toolData),
				EraserSettingsContext{ toolSettingsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::Eraser);
			m_Tools.insert_or_assign(ToolEnum::Eraser, std::make_unique<EraserTool>(eraserTool));
		}

		{ // COLOUR PICKER TOOL
			ToolData toolData{
				.Name = "Colour Picker",
				.KeyShortcut = Walnut::KeyCode::K
			};

			const ColourPickerTool colourPickerTool = ColourPickerTool{
				toolSettingsRegistry->CreateUiDrawer<TSE::PrimaryColour, TSE::SecondaryColour>(),
				toolData,
				ColourPickerSettingsContext{ toolSettingsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::ColourPicker);
			m_Tools.insert_or_assign(ToolEnum::ColourPicker, std::make_unique<ColourPickerTool>(colourPickerTool));
		}
	}

}
