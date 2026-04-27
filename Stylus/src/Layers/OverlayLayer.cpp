#include "OverlayLayer.h"

#include "../Tools/Tool.h"
#include "../Systems/ToolManager.h"

#include <imgui.h>
#include <memory>

namespace Stylus {

	void OverlayLayer::OnUIRender()
	{
		ImGui::Begin("Canvas");

		std::weak_ptr<const Tool> currentTool = m_ToolManager->GetTool();

		if (auto tool = currentTool.lock())
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			tool->DrawOverlayHint(mousePos);
		}

		ImGui::End();
	}

}
