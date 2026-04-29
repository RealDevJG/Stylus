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

	void UiLayer::OnAttach()
	{
		std::fstream fstream("./imgui.ini");

		if (!fstream.good())
		{
			m_ShouldSetDefaultLayout = true;
		}
	}

	void UiLayer::OnUIRender()
	{
		if (m_InitialLoad)
		{
			SetWindowProperties();
			m_InitialLoad = false;
		}

		if (m_ShouldSetDefaultLayout)
		{
			DefaultLayout();
		}

		ImGui::Begin("Tool Bar");

		const auto& tools = m_ToolRegistry->GetTools();
		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			m_UiDrawer.DrawToolButton(toolEnum, toolData.Name);
		}

		ImGui::End();

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

	void UiLayer::SetWindowProperties()
	{
		ImGui::Begin("Canvas"); ImGui::End();
		ImGui::Begin("Tool Bar"); ImGui::End();
		ImGui::Begin("Tool Options"); ImGui::End();

		auto* canvas = ImGui::FindWindowByID(ImHashStr("Canvas"));
		auto* toolOptions = ImGui::FindWindowByID(ImHashStr("Tool Options"));
		auto* toolBar = ImGui::FindWindowByID(ImHashStr("Tool Bar"));

		canvas->WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		toolBar->WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
		toolOptions->WindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	}

	void UiLayer::DefaultLayout()
    {
        ImGuiID dockspaceId = ImGui::GetID("MyDockspace");

		if (ImGui::FindWindowByID(ImHashStr("Canvas")) != nullptr && ImGui::FindWindowByID(ImHashStr("Tool Bar")) != nullptr)
        {
			ImGui::DockBuilderRemoveNode(dockspaceId);
			ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

			SetWindowProperties();

			ImGuiID dockCanvasId = dockspaceId; 
			ImGuiID dockOptionsId = ImGui::DockBuilderSplitNode(dockCanvasId, ImGuiDir_Up, 0.07f, nullptr, &dockCanvasId);
			ImGuiID dockToolsId = ImGui::DockBuilderSplitNode(dockCanvasId, ImGuiDir_Left, 0.11f, nullptr, &dockCanvasId);

			ImGui::DockBuilderDockWindow("Tool Options", dockOptionsId);
			ImGui::DockBuilderDockWindow("Tool Bar", dockToolsId);
			ImGui::DockBuilderDockWindow("Canvas", dockCanvasId);

			ImGui::DockBuilderFinish(dockspaceId);
			m_ShouldSetDefaultLayout = false;
		}
	}

}
