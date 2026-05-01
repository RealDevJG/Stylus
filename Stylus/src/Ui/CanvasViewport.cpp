#include "CanvasViewport.h"

#include <glm/glm.hpp>
#include <imgui_internal.h>

namespace Stylus {

	void CanvasViewport::Render(std::shared_ptr<Walnut::Image> canvasImage)
	{
		m_ViewportOrigin = ImGui::GetCursorScreenPos();
		ImVec2 viewportAvail = ImGui::GetContentRegionAvail();

		if (m_NeedsCentering)
		{
			CentreCanvas(viewportAvail);
			m_NeedsCentering = false;
		}

		ImVec2 bgTopLeft = m_ViewportOrigin;
		ImVec2 bgBottomRight = {
			bgTopLeft.x + viewportAvail.x,
			bgTopLeft.y + viewportAvail.y
		};

		m_CanvasTopLeft = {
			bgTopLeft.x + m_Pan.x,
			bgTopLeft.y + m_Pan.y,
		};

		ImVec2 canvasBottomRight = {
			m_CanvasTopLeft.x + m_CanvasSize.x * m_Scale,
			m_CanvasTopLeft.y + m_CanvasSize.y * m_Scale
		};

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(bgTopLeft, bgBottomRight, IM_COL32(30, 30, 30, 255));
		drawList->AddImage(canvasImage->GetDescriptorSet(), m_CanvasTopLeft, canvasBottomRight);

		m_MousePos = ImGui::GetMousePos();

		ImGui::Dummy(m_CanvasSize);
	}

	void CanvasViewport::ResizeCanvas(uint32_t width, uint32_t height)
	{
		m_CanvasSize.x = static_cast<float>(width);
		m_CanvasSize.y = static_cast<float>(height);

		SetNeedsCentering();
	}

	void CanvasViewport::SetNeedsCentering(bool centre)
	{
		m_NeedsCentering = centre;
	}

	void CanvasViewport::Pan(float dx, float dy)
	{
		static float s_Sensitivity = 128.0f;

		m_Pan.x += dx * s_Sensitivity;
		m_Pan.y += dy * s_Sensitivity;
	}

	void CanvasViewport::Zoom(float dz)
	{
		static float s_Sensitivity = 0.08f;
		float oldZoom = m_Scale;

		m_Scale += dz * (s_Sensitivity + oldZoom * 0.08);
		m_Scale = glm::clamp(m_Scale, 0.1f, 50.0f);

		float ratio = m_Scale / oldZoom;
		m_Pan.x = m_MousePos.x - m_ViewportOrigin.x - (m_MousePos.x - m_ViewportOrigin.x - m_Pan.x) * ratio;
		m_Pan.y = m_MousePos.y - m_ViewportOrigin.y - (m_MousePos.y - m_ViewportOrigin.y - m_Pan.y) * ratio;
	}

	bool CanvasViewport::IsCanvasHovered() const
	{
		return m_MousePos.x > m_CanvasTopLeft.x && m_MousePos.x < m_CanvasTopLeft.x + m_CanvasSize.x * m_Scale
			&& m_MousePos.y > m_CanvasTopLeft.y && m_MousePos.y < m_CanvasTopLeft.y + m_CanvasSize.y * m_Scale;
	}

	float CanvasViewport::GetCanvasScale() const
	{
		return m_Scale;
	}

	glm::vec2 CanvasViewport::ToCanvasSpace(glm::vec2 pos) const
	{
		glm::vec2 transformed = pos;
		transformed.x -= m_Pan.x;
		transformed.y -= m_Pan.y;

		return transformed / m_Scale;
	}

	inline void CanvasViewport::CentreCanvas(ImVec2 viewportAvail)
	{
		m_Pan = {
			(viewportAvail.x - m_CanvasSize.x * m_Scale) / 2.0f,
			(viewportAvail.y - m_CanvasSize.y * m_Scale) / 2.0f
		};
	}

}
