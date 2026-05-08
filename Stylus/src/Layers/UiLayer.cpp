#include "UILayer.h"

#include "../Tools/Tool.h"
#include "../Tools/ToolData.h"
#include "../Tools/ToolEnum.h"

#include "../Layers/CanvasLayer.h"
#include "../Systems/ToolManager.h"
#include "../Systems/ToolRegistry.h"

#include <imgui.h>
#include <Walnut/Application.h>
#include <Walnut/UI/UI.h>

namespace Stylus {

	void UiLayer::OnUIRender()
	{
		static bool s_FirstFrame = true;

		if (m_ResizeCanvasModalOpen)
		{
			DrawResizeCanvasModal();
		}

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
		DrawToolBar();

		ImGui::SetNextWindowClass(&windowClass);
		DrawToolSettings();
	}

	void UiLayer::SetDefaultLayout()
	{
		m_ShouldSetDefaultLayout = true;
	}

	void UiLayer::OpenResizeCanvasModal()
	{
		m_ResizeWidthBuffer = { "854" };
		m_ResizeHeightBuffer = { "480" };
		m_ResizeCanvasModalOpen = true;
		m_ShouldCentreResizeModal = true;
	}

	void UiLayer::DefaultLayout() const
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

	void UiLayer::DrawResizeCanvasModal()
	{
		if (!m_ResizeCanvasModalOpen)
		{
			return;
		}

		ImGui::OpenPopup("Resize Canvas");

		if (m_ShouldCentreResizeModal)
		{
			ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			m_ShouldCentreResizeModal = false;
		}

		m_ResizeCanvasModalOpen = ImGui::BeginPopupModal("Resize Canvas", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

		if (m_ResizeCanvasModalOpen)
		{
			ImGui::InputText("Width", m_ResizeWidthBuffer.data(), m_ResizeWidthBuffer.size(), ImGuiInputTextFlags_CharsDecimal);
			ImGui::InputText("Height", m_ResizeHeightBuffer.data(), m_ResizeHeightBuffer.size(), ImGuiInputTextFlags_CharsDecimal);

			if (Walnut::UI::ButtonCentered("Confirm"))
			{
				m_ResizeCanvasModalOpen = false;
				ImGui::CloseCurrentPopup();

				uint32_t width; uint32_t height;
				std::from_chars(m_ResizeWidthBuffer.data(), m_ResizeWidthBuffer.data() + m_ResizeWidthBuffer.size(), width);
				std::from_chars(m_ResizeHeightBuffer.data(), m_ResizeHeightBuffer.data() + m_ResizeHeightBuffer.size(), height);

				CanvasLayer* canvasLayer = Walnut::Application::Get().GetLayer<CanvasLayer>();
				canvasLayer->ResizeCanvas(width, height);
			}

			if (Walnut::UI::ButtonCentered("Cancel"))
			{
				m_ResizeCanvasModalOpen = false;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	void UiLayer::DrawToolBar() const
	{
		ImGui::Begin("Tool Bar");

		const auto& tools = m_ToolRegistry->GetTools();
		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			m_UiDrawer.DrawToolButton(toolEnum, toolData.Name);
		}

		ImGui::End();
	}

	void UiLayer::DrawToolSettings() const
	{
		ImGui::Begin("Tool Options");

		if (const Tool* currentTool = m_ToolManager->GetTool())
		{
			m_UiDrawer.DrawToolOptions([&currentTool]() { currentTool->DrawOptionsUI(); });
		}

		ImGui::End();
	}

}
