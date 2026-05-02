#include "CanvasLayer.h"

#include "../Tools/Fill/FillCanvasData.h"
#include "../Systems/ToolManager.h"
#include "../Systems/ShaderRegistry.h"

#include <Walnut/Input/KeyCodes.h>
#include <imgui_internal.h>

namespace Stylus {

	static uint32_t g_ComputeQueueFamily = (uint32_t)-1;

	void CanvasLayer::OnAttach()
	{
		uint32_t width = 854;
		uint32_t height = 480;

		CreateCanvas(width, height);
	}

	void CanvasLayer::OnDetach()
	{
		ClearHistory();
	}

	void CanvasLayer::OnEvent(Walnut::Event& event)
	{
		Walnut::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Walnut::KeyPressedEvent>([this](Walnut::KeyPressedEvent& e) { return OnKeyPressed(e); });
		dispatcher.Dispatch<Walnut::KeyReleasedEvent>([this](Walnut::KeyReleasedEvent& e) { return OnKeyReleased(e); });
		dispatcher.Dispatch<Walnut::MousePressedEvent>([this](Walnut::MousePressedEvent& e) { return OnMousePressed(e); });
		dispatcher.Dispatch<Walnut::MouseReleasedEvent>([this](Walnut::MouseReleasedEvent& e) { return OnMouseReleased(e); });
		dispatcher.Dispatch<Walnut::MouseScrolledEvent>([this](Walnut::MouseScrolledEvent& e) { return OnMouseScrolled(e); });
	}

	void CanvasLayer::OnUIRender()
	{
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Canvas", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		m_ViewportHovered = ImGui::IsWindowHovered();
		m_CanvasViewport.Render(m_CanvasImage);
		UpdateMousePos();

		ImGui::End();
	}

	void CanvasLayer::OnUpdate(float ts)
	{
		if (m_SpaceDown)
		{
			if (m_LeftMouseDown || m_RightMouseDown)
			{
				glm::vec2 mouseDelta = (m_MousePos - m_PrevMousePos) * ts;
				m_CanvasViewport.Pan(mouseDelta.x, mouseDelta.y);
			}
		}
		else if (m_ViewportHovered)
		{
			if (m_LeftMouseDown)
			{
				glm::vec2 mousePos = m_CanvasViewport.ToCanvasSpace(m_MousePos);
				glm::vec2 prevMousePos = m_CanvasViewport.ToCanvasSpace(m_PrevMousePos);

				m_ShouldAddToHistory = m_ToolManager->UseLeftClick(mousePos, prevMousePos);
			}
			else if (m_RightMouseDown)
			{
				glm::vec2 mousePos = m_CanvasViewport.ToCanvasSpace(m_MousePos);
				glm::vec2 prevMousePos = m_CanvasViewport.ToCanvasSpace(m_PrevMousePos);

				m_ShouldAddToHistory = m_ToolManager->UseRightClick(mousePos, prevMousePos);
			}
		}
	}

	void CanvasLayer::ClearHistory()
	{
		m_HistoryManager->Clear();
	}

	void CanvasLayer::SaveHistory()
	{
		std::vector<uint8_t> pixels = m_CanvasImage->ReadPixels(0, 0, m_CanvasImage->GetWidth(), m_CanvasImage->GetHeight());
		assert(pixels.size() > 0);

		m_HistoryManager->ActionPerformed(pixels);
	}

	void CanvasLayer::UndoHistory()
	{
		if (auto data = m_HistoryManager->UndoHistory())
		{
			SetCanvasData(data->data());
		}
	}

	void CanvasLayer::RedoHistory()
	{
		if (auto data = m_HistoryManager->RedoHistory())
		{
			SetCanvasData(data->data());
		}
	}

	bool CanvasLayer::OnKeyPressed(Walnut::KeyPressedEvent& event)
	{
		Walnut::KeyCode keyCode = event.GetKeyCode();

		if (keyCode == Walnut::KeyCode::LeftControl)
		{
			m_CtrlDown = true;
		}

		if (keyCode == Walnut::KeyCode::Space)
		{
			m_SpaceDown = true;
		}

		if (m_CtrlDown)
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
		}

		return false;
	}

	bool CanvasLayer::OnKeyReleased(Walnut::KeyReleasedEvent& event)
	{
		Walnut::KeyCode keyCode = event.GetKeyCode();

		if (keyCode == Walnut::KeyCode::LeftControl)
		{
			m_CtrlDown = false;
		}

		if (keyCode == Walnut::KeyCode::Space)
		{
			m_SpaceDown = false;
		}

		return false;
	}

	bool CanvasLayer::OnMousePressed(Walnut::MousePressedEvent& event)
	{
		if (event.GetMouseButton() == Walnut::MouseButton::Left)
		{
			m_LeftMouseDown = true;
		}

		if (event.GetMouseButton() == Walnut::MouseButton::Right)
		{
			m_RightMouseDown = true;
		}

		return true;
	}

	bool CanvasLayer::OnMouseReleased(Walnut::MouseReleasedEvent& event)
	{
		bool handled = false;

		if (m_ShouldAddToHistory)
		{
			SaveHistory();
			handled = true;

			m_ShouldAddToHistory = false;
		}

		if (event.GetMouseButton() == Walnut::MouseButton::Left)
		{
			m_LeftMouseDown = false;
		}

		if (event.GetMouseButton() == Walnut::MouseButton::Right)
		{
			m_RightMouseDown = false;
		}

		return handled;
	}

	bool CanvasLayer::OnMouseScrolled(Walnut::MouseScrolledEvent& event)
	{
		if (m_CtrlDown)
		{
			float dz = event.GetScrollOffset().y;
			m_CanvasViewport.Zoom(dz);
		}

		return false;
	}

	void CanvasLayer::CreateCanvas(uint32_t width, uint32_t height)
	{
		m_CanvasImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
		m_ShaderRegistry->SetCanvasImage(m_CanvasImage);

		FillCanvasPushData pushData{ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };

		auto fillCanvasShader = m_ShaderRegistry->Get(EffectEnum::FillCanvas);
		fillCanvasShader->DispatchShader(&pushData);

		m_CanvasViewport.ResizeCanvas(width, height);
		m_CanvasViewport.Setup(m_CanvasImage);
		SaveHistory();
	}

	void CanvasLayer::ResizeCanvas(uint32_t width, uint32_t height)
	{
		CreateCanvas(width, height);
	}

	float CanvasLayer::GetCanvasScale()
	{
		return m_CanvasViewport.GetCanvasScale();
	}

	void CanvasLayer::SetCanvasData(const void* data) const
	{
		m_CanvasImage->SetData(data);
	}

	void CanvasLayer::UpdateMousePos()
	{
		ImVec2 minImageBounds = ImGui::GetItemRectMin();
		ImVec2 maxImageBounds = ImGui::GetItemRectMax();
		ImVec2 imGuiMousePos = ImGui::GetMousePos();

		float x = imGuiMousePos.x - minImageBounds.x;
		float y = imGuiMousePos.y - minImageBounds.y;

		m_PrevMousePos = m_MousePos;
		m_MousePos = glm::vec2(x, y);
	}

}
