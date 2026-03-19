#include "CanvasLayer.h"

#include <glm/glm.hpp>
#include <iostream>

void CanvasLayer::OnUIRender()
{
	ImGui::Begin("Canvas");

	if (m_CanvasImage)
	{
		ImGui::Image(m_CanvasImage->GetDescriptorSet(), { 512, 512 });
	}

	ImVec2 minImageBounds = ImGui::GetItemRectMin();
	ImVec2 maxImageBounds = ImGui::GetItemRectMax();
	ImVec2 mousePos = ImGui::GetMousePos();

	float x = mousePos.x - minImageBounds.x;
	float y = mousePos.y - minImageBounds.y;

	std::cout << "x: " << x << ", y: " << y << "\n";

	// TODO: dispatch compute shader
	if (ImGui::IsMouseDown(0))
	{
		if (x < 0 || x > m_CanvasImage->GetWidth() - 1 || y < 0 || y > m_CanvasImage->GetHeight() - 1)
		{
			std::cout << "OUT OF BOUNDS CLICK\n";
		}
		else
		{
			std::cout << "IN BOUNDS CLICK\n";

			uint32_t index = x + y * 512;
			m_CanvasData[index] = 255 << 24 | 0 << 16 | 0 << 8 | 255;

			m_CanvasImage->SetData(m_CanvasData.get());
		}
	}

	ImGui::End();
}

void CanvasLayer::OnAttach()
{
	if (!m_CanvasImage)
	{
		m_CanvasImage = std::make_shared<Walnut::Image>(512, 512, Walnut::ImageFormat::RGBA);
	}

	if (!m_CanvasData)
	{
		m_CanvasData = std::make_unique<uint32_t[]>(uint32_t(512 * 512));
	}

	for (uint32_t y = 0; y < 512; ++y)
	{
		for (uint32_t x = 0; x < 512; ++x)
		{
			uint32_t index = x + y * 512;

			uint8_t r = glm::floor(static_cast<double>(x) / 512 * 255);
			uint8_t g = glm::floor(static_cast<double>(y) / 512 * 255);
			uint8_t b = 0;
			uint8_t a = 255;

			auto data = m_CanvasData.get();
			m_CanvasData.get()[index] = a << 24 | b << 16 | g << 8 | r;
		}
	}

	m_CanvasImage->SetData(m_CanvasData.get());
}
