#include "CanvasLayer.h"

#include "../Serialisation/FileReader.h"

#include "../ToolOptions/Brush/BrushShape.h"
#include "../ToolOptions/Brush/BrushPushData.h"
#include "../ToolOptions/Fill/FillCanvasData.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

static uint32_t g_ComputeQueueFamily = (uint32_t)-1;

CanvasLayer::CanvasLayer(std::shared_ptr<Stylus::CoreContext> context)
	: m_Context(context)
{}

CanvasLayer::CanvasLayer(std::shared_ptr<Stylus::CoreContext> context, uint32_t canvasWidth, uint32_t canvasHeight)
	: m_Context(context), m_CanvasWidth(canvasWidth), m_CanvasHeight(canvasHeight)
{}

void CanvasLayer::OnAttach()
{
	if (!m_CanvasImage)
	{
		m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);
		m_Context->ShaderRegistry->SetCanvasImage(m_CanvasImage);

		//m_ToolShader = std::make_unique<Stylus::ComputePipeline>(m_CanvasImage, "assets/shaders/brush.spv", sizeof(Stylus::BrushPushData));
		//m_CanvasFillShader = std::make_unique<Stylus::ComputePipeline>(m_CanvasImage, "assets/shaders/fill-canvas.spv", sizeof(Stylus::FillCanvasPushData));

		Stylus::FillCanvasPushData pushData{};
		pushData.Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		auto fillCanvasShader = m_Context->ShaderRegistry->Get(EffectEnum::FillCanvas);
		fillCanvasShader->DispatchShader(&pushData);

		//m_CanvasFillShader->DispatchShader(&pushData);
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
		m_Context->ShaderRegistry->SetCanvasImage(m_CanvasImage);

		//m_ToolShader = std::make_unique<Stylus::ComputePipeline>(m_CanvasImage, "assets/shaders/brush.spv", sizeof(Stylus::BrushPushData));
		//m_CanvasFillShader = std::make_unique<Stylus::ComputePipeline>(m_CanvasImage, "assets/shaders/fill-canvas.spv", sizeof(Stylus::FillCanvasPushData));

		vkDeviceWaitIdle(Walnut::Application::GetDevice());

		Stylus::FillCanvasPushData pushData{};
		pushData.Colour = glm::vec4(1.0, 1.0, 1.0, 1.0);

		auto fillCanvasShader = m_Context->ShaderRegistry->Get(EffectEnum::FillCanvas);
		fillCanvasShader->DispatchShader(&pushData);

		//m_CanvasFillShader->DispatchShader(&pushData);
	}

	ImGui::Image(m_CanvasImage->GetDescriptorSet(), { static_cast<float>(m_CanvasWidth), static_cast<float>(m_CanvasHeight) });

	ImVec2 minImageBounds = ImGui::GetItemRectMin();
	ImVec2 maxImageBounds = ImGui::GetItemRectMax();
	ImVec2 imGuiMousePos = ImGui::GetMousePos();

	float x = imGuiMousePos.x - minImageBounds.x;
	float y = imGuiMousePos.y - minImageBounds.y;

	glm::vec2 mousePos = glm::vec2(x, y);

	Stylus::BrushPushData pushData{};
	pushData.MousePos = mousePos;
	pushData.Radius = 50;
	pushData.Shape = static_cast<int>(Stylus::BrushShapeEnum::Circle);
	pushData.Antialiased = true;

	if (ImGui::IsMouseDown(0))
	{
		pushData.Colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		m_Context->ToolManager->Use(mousePos);
		//m_ToolShader->DispatchShader(&pushData);
	}
	else if (ImGui::IsMouseDown(1))
	{
		pushData.Colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		m_Context->ToolManager->Use(mousePos);
		//m_ToolShader->DispatchShader(&pushData);
	}

	ImGui::End();
}

bool CanvasLayer::IsInBounds(float x, float y)
{
	return x >= 0 && x < m_CanvasImage->GetWidth() && y >= 0 && y < m_CanvasImage->GetHeight();
}
