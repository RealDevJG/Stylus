#include "CanvasLayer.h"

#include <glm/glm.hpp>

void CanvasLayer::OnUIRender()
{
	ImGui::Begin("Canvas");
	
	if (m_CanvasData)
	{
		ImGui::Image(m_CanvasData->GetDescriptorSet(), { 512, 512 });
	}

	ImGui::End();
}

void CanvasLayer::OnUpdate(float ts)
{
	if (!m_CanvasData)
	{
		m_CanvasData = std::make_shared<Walnut::Image>(512, 512, Walnut::ImageFormat::RGBA);
	}

	uint32_t* data = new uint32_t[512 * 512];

	for (uint32_t y = 0; y < 512; ++y)
	{
		for (uint32_t x = 0; x < 512; ++x)
		{
			uint32_t index = x + y * 512;

			uint8_t r = glm::floor(static_cast<double>(x) / 512 * 255);
			uint8_t g = glm::floor(static_cast<double>(y) / 512 * 255);
			uint8_t b = 0;
			uint8_t a = 255;

			data[index] = a << 24 | b << 16 | g << 8 | r;
		}
	}

	m_CanvasData->SetData(data);
	delete[] data;
}
