#pragma once

#include <glm/vec2.hpp>
#include <Walnut/Image.h>
#include <cstdint>

namespace Stylus {

	class CanvasViewport
	{
	public:
		CanvasViewport() = default;
		~CanvasViewport();

		void Setup(const Walnut::Image& canvasImage, const Walnut::Image& overlayImage);
		void Cleanup();

		void Render();
		void CanvasResized(uint32_t width, uint32_t height);

		void SetNeedsCentering(bool centre = true);
		void SetZoom(float zoom);

		void Pan(glm::vec2 deltas);
		void Zoom(float deltaZoom);

		[[nodiscard]] bool IsCanvasHovered() const;
		[[nodiscard]] float GetCanvasScale() const;
		[[nodiscard]] glm::vec2 GetMousePos() const;
		[[nodiscard]] glm::vec2 ToCanvasSpace(glm::vec2 pos) const;
	private:
		void CentreCanvas(glm::vec2 viewportAvail);
	private:
		VkSampler m_NearestSampler{};
		VkDescriptorSet m_NearestCanvasDescriptorSet{};
		VkDescriptorSet m_NearestOverlayDescriptorSet{};

		glm::vec2 m_ViewportOrigin{};
		glm::vec2 m_CanvasTopLeft{};
		glm::vec2 m_CanvasSize{};
		glm::vec2 m_MousePos{};

		glm::vec2 m_Pan{};
		float m_Zoom = 1.0f;
		bool m_NeedsCentering = true;
	};

}
