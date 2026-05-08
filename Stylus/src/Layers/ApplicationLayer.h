#pragma once

#include <Walnut/Layer.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <memory>

namespace Stylus {

	class ToolManager;
	class ToolRegistry;
	class ToolSettingsController;

	class ApplicationLayer final : public Walnut::Layer
	{
	public:
		ApplicationLayer(
			std::shared_ptr<ToolManager> toolManager,
			std::shared_ptr<ToolRegistry> toolRegistry,
			std::shared_ptr<ToolSettingsController> toolSettingsController
		);

		virtual void OnAttach() override;
		virtual void OnEvent(Walnut::Event& event) override;
	private:
		bool OnKeyPressed(Walnut::KeyPressedEvent& event);
		void SetMenubarCallback() const;
	private:
		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ToolRegistry> m_ToolRegistry;
		std::shared_ptr<ToolSettingsController> m_ToolSettingsController;
	};

}
