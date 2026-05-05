#include "Systems/HistoryManager.h"
#include "Systems/ToolManager.h"
#include "Systems/ToolRegistry.h"
#include "Systems/ToolSettingsRegistry.h"
#include "Systems/ShaderRegistry.h"

#include "Layers/ApplicationLayer.h"
#include "Layers/CanvasLayer.h"
#include "Layers/UILayer.h"
#include "Layers/OverlayLayer.h"

#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include <Walnut/Image.h>
#include <Walnut/UI/UI.h>

#include <memory>
#include <vector>

using namespace Stylus;

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Stylus";
	spec.CustomTitlebar = true;
	spec.CenterWindow = true;
	spec.Width = 1280;
	spec.Height = 720;

	Walnut::Application* app = new Walnut::Application(spec);
	app->SetApplicationIcon(std::make_shared<Walnut::Image>("assets/images/icons/app-icon.png"));

	auto historyManager = std::make_shared<HistoryManager<std::vector<uint8_t>>>();
	auto toolManager = std::make_shared<ToolManager>();
	auto toolSettingsRegistry = std::make_shared<ToolSettingsRegistry>();
	auto toolRegistry = std::make_shared<ToolRegistry>();
	auto shaderRegistry = std::make_shared<ShaderRegistry>();

	toolManager->Init(toolRegistry);
	toolRegistry->Init(toolSettingsRegistry, shaderRegistry);
	shaderRegistry->Init();

	auto applicationLayer = std::make_shared<ApplicationLayer>(toolManager, toolRegistry, shaderRegistry, toolSettingsRegistry);
	auto canvasLayer = std::make_shared<CanvasLayer>(toolManager, shaderRegistry, historyManager);
	auto uiLayer = std::make_shared<UiLayer>(toolManager, toolRegistry);
	auto overlayLayer = std::make_shared<OverlayLayer>(toolManager);

	app->PushLayer(applicationLayer);
	app->PushLayer(canvasLayer);
	app->PushLayer(uiLayer);
	app->PushLayer(overlayLayer);

	app->SetMenubarCallback(
		[app, canvasLayer, uiLayer]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo (ctrl+z)"))
				{
					canvasLayer->UndoHistory();
				}

				if (ImGui::MenuItem("Redo (ctrl+y)"))
				{
					canvasLayer->RedoHistory();
				}

				if (ImGui::MenuItem("Resize Canvas"))
				{
					uiLayer->OpenResizeCanvasModal();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Default Window Layout"))
				{
					uiLayer->SetDefaultLayout();
				}

				ImGui::EndMenu();
			}
		}
	);

	return app;
}
