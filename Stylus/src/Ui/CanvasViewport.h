#pragma once

#include <imgui.h>

namespace Stylus {

	class CanvasViewport
	{
	public:
		CanvasViewport(ImVec2 canvasSize = { 854, 480 });

		void Setup();
		void Render();

		void Pan(float dx, float dy);
		void Zoom(float dz, float mouseX, float mouseY);
		void SetNeedsCentering(bool centre = true);
	private:
		ImVec2 m_CanvasSize;
		ImVec2 m_MousePos{};
		ImVec2 m_Pan{};
		ImVec2 m_ViewportOrigin{};
		float m_Zoom = 1.0f;
		bool m_NeedsCentering = true;
	};

}
