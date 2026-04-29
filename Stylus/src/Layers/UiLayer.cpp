#include "UILayer.h"

#include "../Tools/Tool.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include "../Systems/ToolManager.h"
#include "../Systems/ToolRegistry.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <fstream>

namespace Stylus {

	void UiLayer::OnUIRender()
	{
		static bool s_FirstFrame = true;

		if (s_FirstFrame)
		{
			s_FirstFrame = false;

			if (ImGui::FindWindowSettings(ImHashStr("Canvas")) == nullptr)
			{
				m_ShouldSetDefaultLayout = true;
			}
		}

		if (m_ShouldSetDefaultLayout)
		{
			DefaultLayout();
			m_ShouldSetDefaultLayout = false;
		}

		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Tool Bar");

		const auto& tools = m_ToolRegistry->GetTools();
		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			m_UiDrawer.DrawToolButton(toolEnum, toolData.Name);
		}

		ImGui::End();

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Tool Options");

		std::weak_ptr<const Tool> currentTool = m_ToolManager->GetTool();
		if (auto tool = currentTool.lock())
		{
			m_UiDrawer.DrawToolOptions([&tool]() { tool->DrawOptionsUI(); });
		}

		ImGui::End();
	}

	void UiLayer::SetDefaultLayout()
	{
		m_ShouldSetDefaultLayout = true;
	}

	void UiLayer::DefaultLayout()
    {
        ImGuiID dockspaceId = ImGui::GetID("MyDockspace");

		ImGui::DockBuilderRemoveNode(dockspaceId);
		ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

		ImGuiID dockCanvasId = dockspaceId; 
		ImGuiID dockOptionsId = ImGui::DockBuilderSplitNode(dockCanvasId, ImGuiDir_Up, 0.07f, nullptr, &dockCanvasId);
		ImGuiID dockToolsId = ImGui::DockBuilderSplitNode(dockCanvasId, ImGuiDir_Left, 0.11f, nullptr, &dockCanvasId);

		ImGui::DockBuilderDockWindow("Tool Options", dockOptionsId);
		ImGui::DockBuilderDockWindow("Tool Bar", dockToolsId);
		ImGui::DockBuilderDockWindow("Canvas", dockCanvasId);

		ImGui::DockBuilderFinish(dockspaceId);
	}

}
