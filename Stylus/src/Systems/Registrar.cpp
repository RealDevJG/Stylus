#include "Registrar.h"

#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"
#include "../Vulkan/PushData.h"
#include "../UI/ToolSettingsUI.h"

#include "../Tools/Brush/BrushTool.h"
#include "../Tools/ColourPicker/ColourPickerTool.h"
#include "../Tools/Eraser/EraserTool.h"
#include <Walnut/Application.h>

namespace Stylus {

	namespace TS = Stylus::UI::ToolSettings;

	void Registrar::RegisterToolsAndShaders(ToolRegistry& toolRegistry, ShaderRegistry& shaderRegistry, ToolSettingsStore& settingsStore)
	{
		VkDevice device = Walnut::Application::GetDevice();

		// Tools and Shaders
		{ // BRUSH TOOL
			ToolData toolData{
				"Brush",
				ToolEnum::Brush,
				Walnut::KeyCode::B,
				"assets/shaders/brush-like-tool.spv",
				sizeof(BrushLikePushData),
				"assets/shaders/brush-like-overlay.vert.spv",
				"assets/shaders/brush-like-overlay.frag.spv",
				sizeof(BrushLikeOverlayPushData)
			};

			shaderRegistry.RegisterCompute(ComputeShaderEnum::BrushLikeTool, toolData.ComputeShaderPath, toolData.ComputePushConstantSize);
			shaderRegistry.RegisterGraphics(GraphicsShaderEnum::BrushLikeToolOverlay, toolData.VertShaderPath, toolData.FragShaderPath, toolData.GraphicsPushConstantSize);

			const BrushTool brushTool{
				toolData,
				TS::CreateToolbarSettingsDrawer<TSE::Width, TSE::Shape, TSE::PrimaryColour, TSE::SecondaryColour, TSE::Antialiasing>(settingsStore),
				settingsStore
			};

			toolRegistry.Register<BrushTool>(ToolEnum::Brush, brushTool);
		}

		{ // ERASER TOOL
			ToolData toolData{
				"Eraser",
				ToolEnum::Eraser,
				Walnut::KeyCode::E,
				"assets/shaders/brush-like-tool.spv",
				sizeof(BrushLikePushData),
				"assets/shaders/brush-like-overlay.vert.spv",
				"assets/shaders/brush-like-overlay.frag.spv",
				sizeof(BrushLikeOverlayPushData)
			};

			shaderRegistry.RegisterCompute(ComputeShaderEnum::BrushLikeTool, toolData.ComputeShaderPath, toolData.ComputePushConstantSize);
			shaderRegistry.RegisterGraphics(GraphicsShaderEnum::BrushLikeToolOverlay, toolData.VertShaderPath, toolData.FragShaderPath, toolData.GraphicsPushConstantSize);

			const EraserTool eraserTool{
				toolData,
				TS::CreateToolbarSettingsDrawer<TSE::Width, TSE::Shape, TSE::Antialiasing>(settingsStore),
				settingsStore
			};

			toolRegistry.Register<EraserTool>(ToolEnum::Eraser, eraserTool);
		}

		{ // COLOUR PICKER TOOL
			{
				ToolData toolData{
					"Colour Picker",
					ToolEnum::ColourPicker,
					Walnut::KeyCode::K,
					"",
					0,
					"assets/shaders/brush-like-overlay.vert.spv",
					"assets/shaders/brush-like-overlay.frag.spv",
					sizeof(BrushLikeOverlayPushData)
				};

				shaderRegistry.RegisterGraphics(GraphicsShaderEnum::BrushLikeToolOverlay, toolData.VertShaderPath, toolData.FragShaderPath, toolData.GraphicsPushConstantSize);

				const ColourPickerTool colourPickerTool{
					toolData,
					TS::CreateToolbarSettingsDrawer<TSE::PrimaryColour, TSE::SecondaryColour>(settingsStore),
					settingsStore
				};

				toolRegistry.Register<ColourPickerTool>(ToolEnum::ColourPicker, colourPickerTool);
			}
		}

		// Effect Shaders
		{ // FILL CANVAS EFFECT
			shaderRegistry.RegisterCompute(ComputeShaderEnum::FillCanvasEffect, "assets/shaders/fill-canvas.spv", sizeof(FillCanvasPushData));
		}
	}

}
