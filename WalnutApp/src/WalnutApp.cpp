#include "Layers/ApplicationLayer.h"

#include <memory>

#include <Walnut/Application.h>
#include <Walnut/EntryPoint.h>

#include <Walnut/Image.h>
#include <Walnut/UI/UI.h>

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;

	spec.Name = "Stylus";
	spec.CustomTitlebar = true;
	spec.CenterWindow = true;
	spec.Width = 1280;
	spec.Height = 720;

	Walnut::Application* app = new Walnut::Application(spec);

	std::shared_ptr<Walnut::Image> appIcon = std::make_shared<Walnut::Image>("assets\\images\\icons\\app-icon.png");
	app->SetApplicationIcon(appIcon);

	std::shared_ptr<ApplicationLayer> applicationLayer = std::make_shared<ApplicationLayer>();
	app->PushLayer(applicationLayer);

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
