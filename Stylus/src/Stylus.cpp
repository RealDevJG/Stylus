#include "Layers/EditorLayer.h"

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
	app->PushLayer(std::make_shared<EditorLayer>());

	return app;
}
