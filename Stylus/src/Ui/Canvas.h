#pragma once

#include "ICanvasView.h"

#include <Walnut/Image.h>
#include <memory>

namespace Stylus {

	class Canvas final : public ICanvasView
	{
	public:
		Canvas() = default;
		~Canvas() = default;

		Canvas(const Canvas&) = delete;
		Canvas& operator=(const Canvas&) = delete;
		Canvas(Canvas&&) noexcept = default;
		Canvas& operator=(Canvas&&) noexcept = default;

		void SetData(const void* data);
		[[nodiscard]] const Walnut::Image& GetCanvasImage() const override;
		[[nodiscard]] const Walnut::Image& GetOverlayImage() const override;
	private:
		std::unique_ptr<Walnut::Image> m_CanvasImage;
		std::unique_ptr<Walnut::Image> m_OverlayImage;

		friend class CanvasCoordinator;
		const Walnut::Image& CreateCanvasImage(uint32_t width, uint32_t height);
		const Walnut::Image& CreateOverlayImage(uint32_t width, uint32_t height);
	};

}
