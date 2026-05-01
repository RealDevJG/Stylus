#include "OverlayLayer.h"

#include "../Tools/Tool.h"
#include "../Systems/ToolManager.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <memory>

namespace Stylus {

	void OverlayLayer::OnUIRender()
	{
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		std::weak_ptr<const Tool> currentTool = m_ToolManager->GetTool();

		if (auto tool = currentTool.lock())
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			tool->DrawOverlayHint(mousePos);
		}

		ImGui::End();
	}

}
