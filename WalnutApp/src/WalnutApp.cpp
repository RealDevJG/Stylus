#include "Layers/ApplicationLayer.h"
#include "Layers/CanvasLayer.h"
#include "Layers/UILayer.h"
#include "CoreContext.h"

#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>
#include <Walnut/Image.h>
#include <Walnut/UI/UI.h>

#include <memory>

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

	auto context = std::make_shared<Stylus::CoreContext>();
	context->OptionsRegistry = std::make_shared<Stylus::ToolOptionsRegistry>();
	context->ShaderRegistry = std::make_shared<Stylus::ShaderRegistry>();
	context->ToolManager = std::make_shared<Stylus::ToolManager>(context->ShaderRegistry, context->OptionsRegistry);

	auto applicationLayer = std::make_shared<ApplicationLayer>(context);
	auto canvasLayer = std::make_shared<CanvasLayer>(context);
	auto uiLayer = std::make_shared<UiLayer>(context);

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
