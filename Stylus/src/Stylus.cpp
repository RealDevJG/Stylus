#include "Systems/HistoryManager.h"
#include "Systems/ToolManager.h"
#include "Systems/ToolRegistry.h"
#include "Systems/ToolSettingsController.h"
#include "Systems/ToolSettingsRegistry.h"
#include "Systems/ShaderRegistry.h"

#include "Layers/ApplicationLayer.h"
#include "Layers/CanvasLayer.h"
#include "Layers/UILayer.h"
#include "Layers/OverlayLayer.h"

#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>

#include <memory>

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

	// Systems
	auto shaderRegistry = std::make_shared<ShaderRegistry>();
	auto historyManager = std::make_shared<HistoryManager<std::vector<uint8_t>>>();
	auto toolSettingsRegistry = std::make_shared<ToolSettingsRegistry>();
	auto toolSettingsController = std::make_shared<ToolSettingsController>(toolSettingsRegistry);
	auto toolRegistry = std::make_shared<ToolRegistry>(toolSettingsRegistry, shaderRegistry);
	auto toolManager = std::make_shared<ToolManager>(toolRegistry);

	// Layers
	app->PushLayer(std::make_shared<ApplicationLayer>(toolManager, toolRegistry, toolSettingsController));
	app->PushLayer(std::make_shared<CanvasLayer>(toolManager, shaderRegistry, historyManager));
	app->PushLayer(std::make_shared<UiLayer>(toolManager, toolRegistry));
	app->PushLayer(std::make_shared<OverlayLayer>(toolManager));

	return app;
}
