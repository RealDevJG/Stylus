#include "CanvasViewport.h"

#include "../Utils/ConversionUtils.h"

#include <backends/imgui_impl_vulkan.h>
#include <glm/glm.hpp>
#include <imgui_internal.h>
#include <Walnut/Application.h>

namespace Stylus {

	CanvasViewport::~CanvasViewport()
	{
		Cleanup();
	}

	void CanvasViewport::Setup(const Walnut::Image& canvasImage, const Walnut::Image& overlayImage)
	{
		Cleanup();

		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.maxAnisotropy = 1.0f;
		vkCreateSampler(Walnut::Application::GetDevice(), &samplerInfo, nullptr, &m_NearestSampler);

		m_NearestCanvasDescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_NearestSampler, canvasImage.GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		m_NearestOverlayDescriptorSet = (VkDescriptorSet)ImGui_ImplVulkan_AddTexture(m_NearestSampler, overlayImage.GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		m_Zoom = 1.0f;
		SetNeedsCentering();
	}

	void CanvasViewport::Cleanup()
	{
		vkDestroySampler(Walnut::Application::GetDevice(), m_NearestSampler, nullptr);
	}

	void CanvasViewport::Render()
	{
		m_ViewportOrigin = Utils::ToGlmVec2(ImGui::GetCursorScreenPos());
		glm::vec2 viewportAvail = Utils::ToGlmVec2(ImGui::GetContentRegionAvail());

		if (m_NeedsCentering)
		{
			CentreCanvas(viewportAvail);
			m_NeedsCentering = false;
		}

		glm::vec2 bgTopLeft = m_ViewportOrigin;
		glm::vec2 bgBottomRight = bgTopLeft + viewportAvail;

		m_CanvasTopLeft = glm::floor(bgTopLeft + m_Pan);
		glm::vec2 canvasBottomRight = glm::floor(m_CanvasTopLeft + m_CanvasSize * m_Zoom);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(Utils::ToImVec2(bgTopLeft), Utils::ToImVec2(bgBottomRight), IM_COL32(30, 30, 30, 255));
		drawList->AddImage(m_NearestCanvasDescriptorSet, Utils::ToImVec2(m_CanvasTopLeft), Utils::ToImVec2(canvasBottomRight));
		drawList->AddImage(m_NearestOverlayDescriptorSet, Utils::ToImVec2(m_CanvasTopLeft), Utils::ToImVec2(canvasBottomRight));

		m_MousePos = Utils::ToGlmVec2(ImGui::GetMousePos());
		ImGui::Dummy(Utils::ToImVec2(m_CanvasSize));
	}

	void CanvasViewport::CanvasResized(uint32_t width, uint32_t height)
	{
		m_CanvasSize.x = static_cast<float>(width);
		m_CanvasSize.y = static_cast<float>(height);
		SetNeedsCentering();
	}

	void CanvasViewport::SetNeedsCentering(bool centre)
	{
		m_NeedsCentering = centre;
	}

	void CanvasViewport::SetZoom(float zoom)
	{
		m_Zoom = zoom;
	}

	void CanvasViewport::Pan(glm::vec2 deltas)
	{
		static float s_Sensitivity = 128.0f;
		m_Pan += deltas * s_Sensitivity;
	}

	void CanvasViewport::Zoom(float deltaZoom)
	{
		static float s_Sensitivity = 8.0f;

		float oldZoom = m_Zoom;
		m_Zoom += deltaZoom * (s_Sensitivity + oldZoom * s_Sensitivity * 2.0f);
		m_Zoom = glm::clamp(m_Zoom, 0.1f, 50.0f);

		float ratio = m_Zoom / oldZoom;
		m_Pan = m_MousePos - m_ViewportOrigin - (m_MousePos - m_ViewportOrigin - m_Pan) * ratio;
	}

	void CanvasViewport::CentreCanvas(glm::vec2 viewportAvail)
	{
		m_Pan = (viewportAvail - m_CanvasSize * m_Zoom) / 2.0f;
	}

	bool CanvasViewport::IsCanvasHovered() const
	{
		return m_MousePos.x > m_CanvasTopLeft.x && m_MousePos.x < m_CanvasTopLeft.x + m_CanvasSize.x * m_Zoom
			&& m_MousePos.y > m_CanvasTopLeft.y && m_MousePos.y < m_CanvasTopLeft.y + m_CanvasSize.y * m_Zoom;
	}

	float CanvasViewport::GetCanvasScale() const
	{
		return m_Zoom;
	}

	glm::vec2 CanvasViewport::GetMousePos() const
	{
		return m_MousePos;
	}

	glm::vec2 CanvasViewport::ToCanvasSpace(glm::vec2 pos) const
	{
		return (pos - m_Pan) / m_Zoom;
	}

}
