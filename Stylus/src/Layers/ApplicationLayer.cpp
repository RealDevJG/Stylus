#include "ApplicationLayer.h"

#include "../Systems/ToolManager.h"
#include "../Systems/ToolRegistry.h"
#include "../Systems/ShaderRegistry.h"
#include "../Systems/ToolSettingsRegistry.h"

#include "../Layers/CanvasLayer.h"

#include <Walnut/UI/UI.h>
#include <glm/glm.hpp>

namespace Stylus {

	void ApplicationLayer::OnAttach()
	{
		m_ToolManager->SetTool(ToolEnum::Brush);
	}

	void ApplicationLayer::OnDetach()
	{
		m_ShaderRegistry->Cleanup();
	}

	void ApplicationLayer::OnEvent(Walnut::Event& event)
	{
		Walnut::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Walnut::KeyPressedEvent>([this](Walnut::KeyPressedEvent& e) { return OnKeyPressed(e); });
	}

	bool ApplicationLayer::OnKeyPressed(Walnut::KeyPressedEvent& event)
	{
		Walnut::KeyCode keyCode = event.GetKeyCode();

		// TODO: refactor so the clamping and rounding is done somewhere more central to settings
		if (keyCode == Walnut::KeyCode::LeftBracket)
		{
			auto& value = m_ToolSettingsRegistry->GetValue<TSE::Width>();
			value = glm::floor(glm::clamp(value - 1, 0.0f, 256.0f));

			return true;
		}
		else if (keyCode == Walnut::KeyCode::RightBracket)
		{
			auto& value = m_ToolSettingsRegistry->GetValue<TSE::Width>();
			value = glm::floor(glm::clamp(value + 1, 0.0f, 256.0f));

			return true;
		}

		ToolEnum toolEnum = m_ToolRegistry->GetToolEnum(keyCode);

		if (toolEnum == ToolEnum::None)
		{
			return false;
		}

		m_ToolManager->SetTool(toolEnum);
		return true;
	}

}
