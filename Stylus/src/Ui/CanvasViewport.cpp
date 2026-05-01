#include "CanvasViewport.h"

#include <glm/glm.hpp>
#include <imgui_internal.h>

namespace Stylus {

	CanvasViewport::CanvasViewport(ImVec2 canvasSize)
		: m_CanvasSize(canvasSize) {}

	void CanvasViewport::Setup()
	{
		SetNeedsCentering();
	}

	void CanvasViewport::Render()
	{
		m_ViewportOrigin = ImGui::GetCursorScreenPos();
		ImVec2 viewportAvail = ImGui::GetContentRegionAvail();

		if (m_NeedsCentering)
		{
			m_Pan = {
				(viewportAvail.x - m_CanvasSize.x * m_Zoom) / 2.0f,
				(viewportAvail.y - m_CanvasSize.y * m_Zoom) / 2.0f
			};

			m_NeedsCentering = false;
		}

		ImVec2 bgTopLeft = m_ViewportOrigin;
		ImVec2 bgBottomRight = {
			bgTopLeft.x + viewportAvail.x,
			bgTopLeft.y + viewportAvail.y
		};

		ImVec2 canvasTopLeft = {
			bgTopLeft.x + m_Pan.x,
			bgTopLeft.y + m_Pan.y,
		};

		ImVec2 canvasBottomRight = {
			canvasTopLeft.x + m_CanvasSize.x * m_Zoom,
			canvasTopLeft.y + m_CanvasSize.y * m_Zoom
		};

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(bgTopLeft, bgBottomRight, IM_COL32(30, 30, 30, 255));
		drawList->AddRectFilled(canvasTopLeft, canvasBottomRight, IM_COL32(100, 100, 100, 255));

		m_MousePos = ImGui::GetMousePos();

		ImGui::Dummy(m_CanvasSize);
	}

	void CanvasViewport::Pan(float dx, float dy)
	{
		static float s_Sensitivity = 128.0f;

		m_Pan.x += dx * s_Sensitivity;
		m_Pan.y += dy * s_Sensitivity;
	}

	void CanvasViewport::Zoom(float dz, float mouseX, float mouseY)
	{
		static float s_Sensitivity = 0.08f;
		float oldZoom = m_Zoom;

		m_Zoom += dz * s_Sensitivity;
		m_Zoom = glm::clamp(m_Zoom, 0.05f, 3.5f);

		float ratio = m_Zoom / oldZoom;
		m_Pan.x = m_MousePos.x - m_ViewportOrigin.x - (m_MousePos.x - m_ViewportOrigin.x - m_Pan.x) * ratio;
		m_Pan.y = m_MousePos.y - m_ViewportOrigin.y - (m_MousePos.y - m_ViewportOrigin.y - m_Pan.y) * ratio;
	}

	void CanvasViewport::SetNeedsCentering(bool centre)
	{
		m_NeedsCentering = centre;
	}

}
