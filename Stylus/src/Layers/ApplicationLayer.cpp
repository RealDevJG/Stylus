#include "ApplicationLayer.h"

#include "../Systems/ToolManager.h"
#include "../Systems/ToolRegistry.h"
#include "../Systems/ShaderRegistry.h"

#include "../Layers/CanvasLayer.h"

#include <Walnut/UI/UI.h>

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
		ToolEnum toolEnum = m_ToolRegistry->GetToolEnum(keyCode);

		if (toolEnum == ToolEnum::None)
		{
			return false;
		}

		m_ToolManager->SetTool(toolEnum);
		return true;
	}

}
