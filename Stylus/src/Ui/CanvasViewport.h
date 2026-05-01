#pragma once

#include <glm/glm.hpp>
#include <imgui.h>
#include <Walnut/Image.h>

#include <cstdint>
#include <memory>

namespace Stylus {

	class CanvasViewport
	{
	public:
		void Render(std::shared_ptr<Walnut::Image> canvasImage);
		void ResizeCanvas(uint32_t width, uint32_t height);
		void SetNeedsCentering(bool centre = true);

		void Pan(float dx, float dy);
		void Zoom(float dz);

		[[nodiscard]] bool IsCanvasHovered() const;
		[[nodiscard]] float GetCanvasScale() const;
		[[nodiscard]] glm::vec2 ToCanvasSpace(glm::vec2 pos) const;
	private:
		inline void CentreCanvas(ImVec2 viewportAvail);
	private:
		ImVec2 m_ViewportOrigin{};
		ImVec2 m_CanvasTopLeft{};
		ImVec2 m_CanvasSize{};
		ImVec2 m_MousePos{};

		ImVec2 m_Pan{};
		float m_Scale = 1.0f;

		bool m_NeedsCentering = true;
	};

}
