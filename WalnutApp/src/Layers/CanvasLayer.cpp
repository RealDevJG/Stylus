#include "CanvasLayer.h"

#include <glm/glm.hpp>
#include <iostream>

CanvasLayer::CanvasLayer(uint32_t canvasWidth, uint32_t canvasHeight)
	: m_CanvasWidth(canvasWidth), m_CanvasHeight(canvasHeight) {}

void CanvasLayer::OnUIRender()
{
	ImGui::Begin("Canvas");

	if (m_CanvasImage)
	{
		ImGui::Image(m_CanvasImage->GetDescriptorSet(), { static_cast<float>(m_CanvasWidth), static_cast<float>(m_CanvasHeight) });
	}

	ImVec2 minImageBounds = ImGui::GetItemRectMin();
	ImVec2 maxImageBounds = ImGui::GetItemRectMax();
	ImVec2 mousePos = ImGui::GetMousePos();

	float x = mousePos.x - minImageBounds.x;
	float y = mousePos.y - minImageBounds.y;

	// TODO: dispatch compute shader
	if (ImGui::IsMouseDown(0))
	{
		uint32_t colour = 255 << 24 | 0 << 16 | 0 << 8 | 255;
		DrawWithRadius(x, y, 5, colour);
	}
	else if (ImGui::IsMouseDown(1))
	{
		uint32_t colour = 255 << 24 | 255 << 16 | 0 << 8 | 0;
		DrawWithRadius(x, y, 5, colour);
	}

	ImGui::End();
}

void CanvasLayer::DrawWithRadius(int32_t centreX, int32_t centreY, int radius, uint32_t colour)
{
	int32_t xMin = centreX - radius;
	int32_t xMax = centreX + radius;

	int32_t yMin = centreY - radius;
	int32_t yMax = centreY + radius;

	for (int32_t y = yMin; y < yMax; ++y)
	{
		for (int32_t x = xMin; x < xMax; ++x)
		{
			if (IsInBounds(x, y))
			{
				uint32_t index = static_cast<uint32_t>(x + y * m_CanvasWidth);
				m_CanvasData[index] = colour;

				m_CanvasImage->SetData(m_CanvasData.get());
			}
		}
	}
}

void CanvasLayer::OnAttach()
{
	if (!m_CanvasImage)
	{
		m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);
	}

	if (!m_CanvasData)
	{
		m_CanvasData = std::make_unique<uint32_t[]>(uint32_t(m_CanvasWidth * m_CanvasHeight));
	}

	for (uint32_t y = 0; y < m_CanvasHeight; ++y)
	{
		for (uint32_t x = 0; x < m_CanvasWidth; ++x)
		{
			uint32_t index = x + y * m_CanvasWidth;

			uint8_t r = glm::floor(static_cast<double>(x) / m_CanvasWidth * 255);
			uint8_t g = glm::floor(static_cast<double>(y) / m_CanvasHeight * 255);
			uint8_t b = 0;
			uint8_t a = 255;

			auto data = m_CanvasData.get();
			m_CanvasData.get()[index] = a << 24 | b << 16 | g << 8 | r;
		}
	}

	m_CanvasImage->SetData(m_CanvasData.get());
}

bool CanvasLayer::IsInBounds(float x, float y)
{
	return x >= 0 && x < m_CanvasImage->GetWidth() && y >= 0 && y < m_CanvasImage->GetHeight();
}
