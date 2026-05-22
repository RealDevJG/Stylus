#include "Canvas.h"

namespace Stylus {

	void Canvas::SetData(const void* data)
	{
		m_CanvasImage->SetData(data);
	}

	const Walnut::Image& Canvas::GetCanvasImage() const
	{
		return *m_CanvasImage;
	}

	const Walnut::Image& Canvas::CreateCanvasImage(uint32_t width, uint32_t height)
	{
		m_CanvasImage = std::make_unique<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
		return GetCanvasImage();
	}

}
