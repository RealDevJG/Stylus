#include "ImageFileOperations.h"

#include "../Utils/FileUtils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <filesystem>
#include <iostream>

namespace Stylus {

    ImageFileOperations::ImageFileOperations(Canvas& canvas, CanvasCoordinator& canvasCoordinator)
        : m_Canvas(canvas), m_CanvasCoordinator(canvasCoordinator) {}

    void ImageFileOperations::Open()
    {
        std::string imagePath = Utils::OpenFileDialog();

        if (imagePath != "")
        {
            auto& canvasImage = m_Canvas.SetCanvasImage(imagePath);

            m_CanvasCoordinator.SetupCanvases(m_Canvas.GetCanvasImage(), m_Canvas.GetOverlayImage());
            m_CanvasCoordinator.ClearHistory();
            m_CanvasCoordinator.SaveHistory();
        }
    }

    void ImageFileOperations::SaveAs()
    {
        const Walnut::Image& canvasImage = m_Canvas.GetCanvasImage();
        auto pixelData = canvasImage.ReadPixels(0, 0, canvasImage.GetWidth(), canvasImage.GetHeight());

        if (!SaveAsImpl(canvasImage.GetWidth(), canvasImage.GetHeight(), pixelData.data()))
        {
            std::cerr << "Failed to save file, supported extensions: png, jpg, bmp\n";
        }
    }

    bool ImageFileOperations::SaveAsImpl(uint32_t width, uint32_t height, const void* fileData)
    {
        std::string rawPath = Utils::SaveFileDialog();
        std::filesystem::path path{ rawPath };

        std::filesystem::path extFp = path.extension();
        std::string ext = extFp.string();

        if (path.empty())
        {
            return true; // cancelled so don't complain that an unsupported file extension was used
        }

        if (ext == ".png" || ext == ".PNG")
        {
            stbi_write_png(path.string().c_str(), width, height, 4, fileData, 4 * width * sizeof(unsigned char));
            return true;
        }
        else if (ext == ".jpg" || ext == ".jpeg" || ext == ".JPG" || ext == ".JPEG")
        {
            stbi_write_jpg(path.string().c_str(), width, height, 4, fileData, 90);
            return true;
        }
        else if (ext == ".bmp" || ext == ".BMP")
        {
            stbi_write_bmp(path.string().c_str(), width, height, 4, fileData);
            return true;
        }

        return false;
    }

}
