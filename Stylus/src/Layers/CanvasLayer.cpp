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
		// TODO: this is temporary, remove after fixed canvas sizes have been added (needed rn bc OnAttach fires 3 times)
		// The reason it fires 3 times is because the UiLayer's ImGui panels for the toolbar and tool settings get added later, so canvas needs to resize
		static uint8_t s_Ticked = 0;

		m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);
		m_ShaderRegistry->SetCanvasImage(m_CanvasImage);

		FillCanvasPushData pushData{ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };

		auto fillCanvasShader = m_ShaderRegistry->Get(EffectEnum::FillCanvas);
		fillCanvasShader->DispatchShader(&pushData);

		// TODO: this is temporary, remove after fixed canvas sizes have been added (needed rn bc OnAttach fires 3 times)
		// The reason it fires 3 times is because the UiLayer's ImGui panels for the toolbar and tool settings get added later, so canvas needs to resize
		if (++s_Ticked >= 3)
		{
			SaveHistory();
		}
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
	}

	void CanvasLayer::OnUIRender()
	{
		ImGuiWindowClass windowClass;
		windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

		ImGui::SetNextWindowClass(&windowClass);
		ImGui::Begin("Canvas");

		const uint32_t width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		const uint32_t height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

		if (!m_CanvasImage || m_CanvasImage->GetWidth() != width || m_CanvasImage->GetHeight() != height)
		{
			m_CanvasWidth = width;
			m_CanvasHeight = height;

			OnAttach();
		}

		ImGui::Image(m_CanvasImage->GetDescriptorSet(), { static_cast<float>(m_CanvasWidth), static_cast<float>(m_CanvasHeight) });
		m_CanvasHovered = ImGui::IsItemHovered();

		ImVec2 minImageBounds = ImGui::GetItemRectMin();
		ImVec2 maxImageBounds = ImGui::GetItemRectMax();
		ImVec2 imGuiMousePos = ImGui::GetMousePos();

		float x = imGuiMousePos.x - minImageBounds.x;
		float y = imGuiMousePos.y - minImageBounds.y;

		m_MousePos = glm::vec2(x, y);

		ImGui::End();
	}

	void CanvasLayer::OnUpdate(float ts)
	{
		if (m_LeftMouseDown)
		{
			m_CanvasHistoried = m_ToolManager->UseLeftClick(m_MousePos, m_PrevMousePos);
		}
		else if (m_RightMouseDown)
		{
			m_CanvasHistoried = m_ToolManager->UseRightClick(m_MousePos, m_PrevMousePos);
		}

		m_PrevMousePos = m_MousePos;
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

		return false;
	}

	bool CanvasLayer::OnMousePressed(Walnut::MousePressedEvent& event)
	{
		if (!m_CanvasHovered)
		{
			return false;
		}

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

		if (m_CanvasHistoried)
		{
			SaveHistory();
			handled = true;

			m_CanvasHistoried = false;
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

	void CanvasLayer::SetCanvasData(const void* data) const
	{
		m_CanvasImage->SetData(data);
	}

}
