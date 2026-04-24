#include "ToolRegistry.h"

#include "Brush/BrushTool.h"
#include "Brush/BrushPushData.h"
#include "ColourPicker/ColourPickerTool.h"
#include "Eraser/EraserTool.h"
#include "Eraser/EraserPushData.h"

#include "ToolSettingsEnum.h"
#include "ToolSettingsRegistry.h"
#include "../Vulkan/ShaderRegistry.h"

namespace Stylus {

	void ToolRegistry::Init(std::shared_ptr<ToolSettingsRegistry> toolSettingsRegistry, std::shared_ptr<ShaderRegistry> shaderRegistry)
	{
		// TODO: might not need members at all
		m_ToolSettingsRegistry = toolSettingsRegistry;
		m_ShaderRegistry = shaderRegistry;

		{
			ToolData toolData{
				"Brush",
				"assets/shaders/brush.spv",
				sizeof(BrushPushData),
				Walnut::KeyCode::B
			};

			const BrushTool brushTool = BrushTool{
				m_ToolSettingsRegistry->CreateUiDrawer<TSE::Width, TSE::Shape, TSE::PrimaryColour, TSE::SecondaryColour, TSE::Antialiasing>(),
				toolData,
				m_ShaderRegistry->RegisterAndGet(ToolEnum::Brush, toolData),
				BrushSettingsContext{ m_ToolSettingsRegistry }
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
				m_ToolSettingsRegistry->CreateUiDrawer<TSE::Width, TSE::Shape, TSE::Antialiasing>(),
				toolData,
				m_ShaderRegistry->RegisterAndGet(ToolEnum::Eraser, toolData),
				EraserSettingsContext{ m_ToolSettingsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::Eraser);
			m_Tools.insert_or_assign(ToolEnum::Eraser, std::make_shared<EraserTool>(eraserTool));
		}

		{
			ToolData toolData{};
			toolData.Name = "Colour Picker";
			toolData.KeyShortcut = Walnut::KeyCode::I;

			const ColourPickerTool colourPickerTool = ColourPickerTool{
				m_ToolSettingsRegistry->CreateUiDrawer<TSE::PrimaryColour, TSE::SecondaryColour>(),
				toolData,
				ColourPickerSettingsContext{ m_ToolSettingsRegistry }
			};

			m_KeyShortcuts.insert_or_assign(toolData.KeyShortcut, ToolEnum::ColourPicker);
			m_Tools.insert_or_assign(ToolEnum::ColourPicker, std::make_shared<ColourPickerTool>(colourPickerTool));
		}
	}

	std::shared_ptr<const Tool> ToolRegistry::GetTool(ToolEnum tool) const
	{
		return m_Tools.at(tool);
	}

	ToolEnum ToolRegistry::GetToolEnum(Walnut::KeyCode shortcut) const
	{
		auto it = m_KeyShortcuts.find(shortcut);

		if (it != m_KeyShortcuts.end())
			return it->second;

		return ToolEnum::None;
	}

	const std::unordered_map<Stylus::ToolEnum, std::shared_ptr<const Tool>>& ToolRegistry::GetTools() const
	{
		return m_Tools;
	}

}
