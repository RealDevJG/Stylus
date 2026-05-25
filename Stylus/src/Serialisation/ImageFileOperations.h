#pragma once

#include "../UI/Canvas.h"
#include "../UI/CanvasCoordinator.h"

#include <Walnut/Image.h>
#include <memory>

namespace Stylus {

	class ImageFileOperations
	{
	public:
		ImageFileOperations(Canvas& canvas, CanvasCoordinator& canvasCoordinator);

		void Open();
		void SaveAs();
		bool SaveAsImpl(uint32_t width, uint32_t height, const void* fileData);
	private:
		Canvas& m_Canvas;
		CanvasCoordinator& m_CanvasCoordinator;
	};

}
