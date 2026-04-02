#include "CanvasLayer.h"

#include "../Serialisation/FileReader.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

static uint32_t g_ComputeQueueFamily = (uint32_t)-1;

CanvasLayer::CanvasLayer(uint32_t canvasWidth, uint32_t canvasHeight)
	: m_CanvasWidth(canvasWidth), m_CanvasHeight(canvasHeight)
{}

void CanvasLayer::OnAttach()
{
	if (!m_CanvasImage)
	{
		m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);
		m_ComputePipeline = std::make_unique<Stylus::ComputePipeline>(m_CanvasImage);
	}
}

void CanvasLayer::OnUIRender()
{
	ImGui::Begin("Canvas");

	const uint32_t width = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
	const uint32_t height = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);

	if (!m_CanvasImage || m_CanvasImage->GetWidth() != width || m_CanvasImage->GetHeight() != height)
	{
		m_CanvasWidth = width;
		m_CanvasHeight = height;

		vkDeviceWaitIdle(Walnut::Application::GetDevice());

		m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);
		m_ComputePipeline = std::make_unique<Stylus::ComputePipeline>(m_CanvasImage);
	}

	ImGui::Image(m_CanvasImage->GetDescriptorSet(), { static_cast<float>(m_CanvasWidth), static_cast<float>(m_CanvasHeight) });
	m_ComputePipeline->DispatchShader(m_CanvasImage->GetWidth(), m_CanvasImage->GetHeight());

	ImGui::End();
}

void CanvasLayer::DrawWithRadius(int32_t centreX, int32_t centreY, int radius, uint32_t colour)
{
	//int32_t xMin = centreX - radius;
	//int32_t xMax = centreX + radius;

	//int32_t yMin = centreY - radius;
	//int32_t yMax = centreY + radius;

	//for (int32_t y = yMin; y < yMax; ++y)
	//{
	//	for (int32_t x = xMin; x < xMax; ++x)
	//	{
	//		if (IsInBounds(x, y))
	//		{
	//			uint32_t index = static_cast<uint32_t>(x + y * m_CanvasWidth);
	//			m_CanvasData[index] = colour;

	//			m_CanvasImage->SetData(m_CanvasData.get());
	//		}
	//	}
	//}
}

bool CanvasLayer::IsInBounds(float x, float y)
{
	return x >= 0 && x < m_CanvasImage->GetWidth() && y >= 0 && y < m_CanvasImage->GetHeight();
}
