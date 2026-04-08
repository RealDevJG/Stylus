#include "CanvasLayer.h"

#include "../Serialisation/FileReader.h"
#include "../Tools/Options/Brush/BrushShape.h"
#include "../Tools/Options/Brush/BrushPushData.h"
#include "../Tools/Options/Fill/FillCanvasData.h"

#include <vector>

static uint32_t g_ComputeQueueFamily = (uint32_t)-1;

CanvasLayer::CanvasLayer(std::shared_ptr<Stylus::CoreContext> context)
	: m_Context(context) {}

CanvasLayer::CanvasLayer(std::shared_ptr<Stylus::CoreContext> context, uint32_t canvasWidth, uint32_t canvasHeight)
	: m_Context(context), m_CanvasWidth(canvasWidth), m_CanvasHeight(canvasHeight) {}

void CanvasLayer::OnAttach()
{
	m_CanvasImage = std::make_shared<Walnut::Image>(m_CanvasWidth, m_CanvasHeight, Walnut::ImageFormat::RGBA);
	m_Context->ShaderRegistry->SetCanvasImage(m_CanvasImage);

	vkDeviceWaitIdle(Walnut::Application::GetDevice());

	Stylus::FillCanvasPushData pushData{};
	pushData.Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	auto fillCanvasShader = m_Context->ShaderRegistry->Get(Stylus::EffectEnum::FillCanvas);
	fillCanvasShader->DispatchShader(&pushData);
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

		OnAttach();
	}

	ImGui::Image(m_CanvasImage->GetDescriptorSet(), { static_cast<float>(m_CanvasWidth), static_cast<float>(m_CanvasHeight) });
	m_IsCanvasHovered = ImGui::IsItemHovered();

	ImVec2 minImageBounds = ImGui::GetItemRectMin();
	ImVec2 maxImageBounds = ImGui::GetItemRectMax();
	ImVec2 imGuiMousePos = ImGui::GetMousePos();

	float x = imGuiMousePos.x - minImageBounds.x;
	float y = imGuiMousePos.y - minImageBounds.y;

	m_MousePos = glm::vec2(x, y);

	ImGui::End();
}

void CanvasLayer::OnUpdate(float ts)
{
	if (ImGui::IsMouseClicked(0) && m_IsCanvasHovered)
	{
		m_LeftMouseDown = true;
	}
	else if (ImGui::IsMouseClicked(1) && m_IsCanvasHovered)
	{
		m_RightMouseDown = true;
	}

	if (ImGui::IsMouseReleased(0))
	{
		m_LeftMouseDown = false;
	}

	if (ImGui::IsMouseReleased(1))
	{
		m_RightMouseDown = false;
	}

	if (m_LeftMouseDown)
	{
		m_Context->ToolManager->Use(m_MousePos, ImGuiMouseButton_Left);
	}
	else if (m_RightMouseDown)
	{
		m_Context->ToolManager->Use(m_MousePos, ImGuiMouseButton_Right);
	}
}
