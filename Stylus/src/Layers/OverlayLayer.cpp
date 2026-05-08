#include "OverlayLayer.h"

#include "../Tools/Tool.h"
#include "../Layers/CanvasLayer.h"
#include "../Systems/ToolManager.h"

#include <Walnut/Application.h>
#include <imgui.h>
#include <imgui_internal.h>

namespace Stylus {

	void OverlayLayer::OnUIRender()
	{
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (const Tool* currentTool = m_ToolManager->GetTool())
		{
			CanvasLayer* canvasLayer = Walnut::Application::Get().GetLayer<CanvasLayer>();
			float scale = canvasLayer->GetCanvasScale();

			ImVec2 mousePos = ImGui::GetMousePos();
			currentTool->DrawOverlayHint(mousePos, scale);
		}

		ImGui::End();
	}

}
