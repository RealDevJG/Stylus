#include "OverlayLayer.h"

#include "../Tools/Tool.h"
#include "../Layers/CanvasLayer.h"
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
			CanvasLayer* canvasLayer = Walnut::Application::Get().GetLayer<CanvasLayer>();
			float scale = canvasLayer->GetCanvasScale();

			ImVec2 mousePos = ImGui::GetMousePos();
			tool->DrawOverlayHint(mousePos, scale);
		}

		ImGui::End();
	}

}
