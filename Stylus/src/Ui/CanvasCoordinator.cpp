#include "CanvasCoordinator.h"

#include "../Tools/ToolActions.h"
#include "../Vulkan/PushData.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <Walnut/Application.h>

namespace Stylus {

	CanvasCoordinator::CanvasCoordinator(Canvas& canvas, CanvasViewport& canvasViewport, HistoryManager<std::vector<uint8_t>>& historyManager, const ToolActionExecutor& actionExecutor, IShaderImageUpdater& shaderImageUpdater)
		: m_Canvas(canvas), m_CanvasViewport(canvasViewport), m_HistoryManager(historyManager), m_ActionExecutor(actionExecutor), m_ShaderImageUpdater(shaderImageUpdater) {}

	void CanvasCoordinator::CreateCanvas(uint32_t width, uint32_t height)
	{
		const Walnut::Image& canvasImage = m_Canvas.CreateCanvasImage(width, height);
		const Walnut::Image& overlayImage = m_Canvas.CreateOverlayImage(width, height);

		FillCanvasPushData pushData{ glm::vec4{ 1.0f } };
		ToolAction action{
			ToolActionType::ComputeShaderAction,
			ComputeShaderAction{
				.ShaderEnum = ComputeShaderEnum::FillCanvasEffect,
				.PushData = Utils::CopyToBytes(pushData),
				.PushDataSize = sizeof(pushData)
			}
		};

		SetupCanvases(canvasImage, overlayImage);
		m_ActionExecutor.Execute(action);

		ClearHistory();
		SaveHistory();
	}

	void CanvasCoordinator::SetupCanvases(const Walnut::Image& canvasImage, const Walnut::Image& overlayImage)
	{
		VkDevice device = Walnut::Application::Get().GetDevice();

		m_ShaderImageUpdater.UpdateStorageImage(canvasImage.GetImageView());
		m_ShaderImageUpdater.UpdateFramebuffers(device, overlayImage.GetImageView(), overlayImage.GetWidth(), overlayImage.GetHeight());

		m_CanvasViewport.CanvasResized(canvasImage.GetWidth(), canvasImage.GetHeight());
		m_CanvasViewport.Setup(canvasImage, overlayImage);
	}

	void CanvasCoordinator::Render()
	{
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Canvas Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		m_ViewportHovered = ImGui::IsWindowHovered();
		m_CanvasViewport.Render();
		UpdateMousePos();

		ImGui::End();
	}

	void CanvasCoordinator::ClearHistory()
	{
		m_HistoryManager.Clear();
	}

	void CanvasCoordinator::SaveHistory()
	{
		const Walnut::Image& canvasImage = m_Canvas.GetCanvasImage();

		auto pixels = canvasImage.ReadPixels(0, 0, canvasImage.GetWidth(), canvasImage.GetHeight());
		assert(pixels.size() > 0);

		m_HistoryManager.ActionPerformed(pixels);
	}

	void CanvasCoordinator::UndoHistory()
	{
		if (auto data = m_HistoryManager.Undo())
		{
			m_Canvas.SetData(data->data());
		}
	}

	void CanvasCoordinator::RedoHistory()
	{
		if (auto data = m_HistoryManager.Redo())
		{
			m_Canvas.SetData(data->data());
		}
	}

	bool CanvasCoordinator::OnKeyPressed(Walnut::KeyCode keyCode)
	{
		if (keyCode == Walnut::KeyCode::Z)
		{
			UndoHistory();
			return true;
		}
		else if (keyCode == Walnut::KeyCode::Y)
		{
			RedoHistory();
			return true;
		}

		if (keyCode == Walnut::KeyCode::D0 || keyCode == Walnut::KeyCode::KP0)
		{
			m_CanvasViewport.SetZoom(1.0f);
			m_CanvasViewport.SetNeedsCentering();
			return true;
		}

		return false;
	}

	void CanvasCoordinator::OnMouseScrolled(glm::vec2 scrollOffset, float ts)
	{
		float deltaZoom = scrollOffset.y * ts;
		m_CanvasViewport.Zoom(deltaZoom);
	}

	void CanvasCoordinator::OnMouseMoved(glm::vec2 screenMousePos, glm::vec2 prevScreenMousePos, float ts)
	{
		glm::vec2 mouseDelta = (screenMousePos - prevScreenMousePos) * ts;
		m_CanvasViewport.Pan(mouseDelta);
	}

	bool CanvasCoordinator::IsCanvasViewportHovered() const
	{
		return m_ViewportHovered;
	}

	glm::vec2 CanvasCoordinator::GetCanvasMousePos() const
	{
		return m_MousePos;
	}

	float CanvasCoordinator::GetCanvasScale() const
	{
		return m_CanvasViewport.GetCanvasScale();
	}

	void CanvasCoordinator::UpdateMousePos()
	{
		ImVec2 minImageBounds = ImGui::GetItemRectMin();
		ImVec2 maxImageBounds = ImGui::GetItemRectMax();
		ImVec2 imGuiMousePos = ImGui::GetMousePos();

		float x = imGuiMousePos.x - minImageBounds.x;
		float y = imGuiMousePos.y - minImageBounds.y;

		m_MousePos = m_CanvasViewport.ToCanvasSpace(glm::vec2(x, y));
	}

}
