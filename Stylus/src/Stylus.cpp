#include "Tools/ToolManager.h"
#include "Tools/ToolRegistry.h"
#include "Tools/ToolSettingsRegistry.h"
#include "Vulkan/ShaderRegistry.h"

#include "Layers/ApplicationLayer.h"
#include "Layers/CanvasLayer.h"
#include "Layers/UILayer.h"

#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include <Walnut/Image.h>
#include <Walnut/UI/UI.h>

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

	auto toolManager = std::make_shared<ToolManager>();
	auto toolSettingsRegistry = std::make_shared<ToolSettingsRegistry>();
	auto toolRegistry = std::make_shared<ToolRegistry>();
	auto shaderRegistry = std::make_shared<ShaderRegistry>();

	toolManager->Init(toolRegistry);
	toolRegistry->Init(toolSettingsRegistry, shaderRegistry);
	shaderRegistry->Init();

	auto applicationLayer = std::make_shared<ApplicationLayer>(toolManager, toolRegistry, shaderRegistry);
	auto canvasLayer = std::make_shared<CanvasLayer>(toolManager, shaderRegistry);
	auto uiLayer = std::make_shared<UiLayer>(toolManager, toolRegistry);

	app->PushLayer(applicationLayer);
	app->PushLayer(canvasLayer);
	app->PushLayer(uiLayer);

	app->SetMenubarCallback([app, applicationLayer]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About"))
				{
					applicationLayer->ShowAboutModal();
				}

				ImGui::EndMenu();
			}
		});

	return app;
}
