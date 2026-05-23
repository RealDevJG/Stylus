#include "UIManager.h"

#include "../Tools/Tool.h"
#include "../Tools/ToolData.h"
#include "../Utils/ConversionUtils.h"

#include <imgui.h>
#include <Walnut/UI/UI.h>

namespace Stylus {

	UIManager::UIManager(IToolManagerState& toolManagerState, IToolRegistryReadonly& toolRegistryReadOnly, ICanvasContext& canvasContext, const ToolActionExecutor& actionExecutor)
		: m_ToolManagerState(toolManagerState), m_ToolRegistryReadonly(toolRegistryReadOnly), m_CanvasContext(canvasContext), m_ActionExecutor(actionExecutor) {}

	void UIManager::Render()
	{
		static bool s_FirstFrame = true;

		if (s_FirstFrame)
		{
			s_FirstFrame = false;

			if (ImGui::FindWindowSettings(ImHashStr("Canvas Viewport")) == nullptr)
			{
				m_ShouldSetDefaultLayout = true;
			}
		}

		if (m_ResizeCanvasModalOpen)
		{
			DrawResizeCanvasModal();
		}

		if (m_ShouldSetDefaultLayout)
		{
			DefaultLayout();
			m_ShouldSetDefaultLayout = false;
		}

		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		// Tool bar
		ImGui::SetNextWindowClass(&windowClass);
		DrawToolBar();

		// Tool settings bar
		ImGui::SetNextWindowClass(&windowClass);
		DrawToolSettings();

		// Tool overlay e.g. circle outline of where brush is going to draw
		ImGui::SetNextWindowClass(&windowClass);
		DrawToolOverlayHint();
	}

	void UIManager::SetDefaultLayout()
	{
		m_ShouldSetDefaultLayout = true;
	}

	void UIManager::OpenResizeCanvasModal()
	{
		m_ResizeWidthBuffer = { "854" };
		m_ResizeHeightBuffer = { "480" };
		m_ResizeCanvasModalOpen = true;
		m_ShouldCentreResizeModal = true;
	}

	void UIManager::DefaultLayout() const
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
		ImGui::DockBuilderDockWindow("Canvas Viewport", dockCanvasId);

		ImGui::DockBuilderFinish(dockspaceId);
	}

	void UIManager::DrawResizeCanvasModal()
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

				m_CanvasContext.CreateCanvas(width, height);
			}

			if (Walnut::UI::ButtonCentered("Cancel"))
			{
				m_ResizeCanvasModalOpen = false;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	void UIManager::DrawToolBar() const
	{
		ImGui::Begin("Tool Bar");

		const auto& tools = m_ToolRegistryReadonly.GetTools();
		for (const auto& [toolEnum, tool] : tools)
		{
			const ToolData& toolData = tool->GetToolData();
			DrawToolButton(toolEnum, toolData.Name);
		}

		ImGui::End();
	}

	void UIManager::DrawToolSettings() const
	{
		ImGui::Begin("Tool Options");

		if (const Tool* currentTool = m_ToolManagerState.GetCurrentTool())
		{
			currentTool->DrawSettingsUI();
		}

		ImGui::End();
	}

	void UIManager::DrawToolOverlayHint() const
	{
		ImGui::Begin("Canvas Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (const Tool* currentTool = m_ToolManagerState.GetCurrentTool())
		{
			float scale = m_CanvasContext.GetCanvasScale();
			glm::vec2 mousePos = m_CanvasContext.GetCanvasMousePos();

			ToolAction action = currentTool->DrawOverlayHint(mousePos, scale);
			m_ActionExecutor.Execute(action);
		}

		ImGui::End();
	}

	void UIManager::DrawToolButton(ToolEnum toolEnum, std::string_view toolName) const
	{
		const float width = ImGui::GetContentRegionAvail().x;

		if (ImGui::Button(toolName.data(), ImVec2(width, 35)))
		{
			m_ToolManagerState.SetTool(toolEnum);
		}
	}

}
