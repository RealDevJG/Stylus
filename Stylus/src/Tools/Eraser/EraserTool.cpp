#include "EraserTool.h"

#include "EraserPushData.h"
#include "../../Vulkan/ComputeShader.h"

namespace Stylus {

	EraserTool::EraserTool(std::function<void()> drawUiStrategy, const ToolData& toolData, const ComputeShader* shader, EraserSettingsContext context)
		: Tool(drawUiStrategy, toolData), m_Shader(shader), m_SettingsContext(context) {}

	bool EraserTool::UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
		if (m_Shader)
		{
			EraserPushData pushData{
				mousePos,
				prevMousePos,
				static_cast<int>(m_SettingsContext.Get<TSE::Shape>()),
				m_SettingsContext.Get<TSE::Width>(),
				m_SettingsContext.Get<TSE::Antialiasing>() ? 1u : 0u
			};

			m_Shader->DispatchShader(&pushData);
			return true;
		}

		return false;
	}

	bool EraserTool::UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
		if (m_Shader)
		{
			EraserPushData pushData{
				mousePos,
				prevMousePos,
				static_cast<int>(m_SettingsContext.Get<TSE::Shape>()),
				m_SettingsContext.Get<TSE::Width>(),
				m_SettingsContext.Get<TSE::Antialiasing>() ? 1u : 0u
			};

			m_Shader->DispatchShader(&pushData);
			return true;
		}

		return false;
	}

	void EraserTool::DrawOverlayHint(const ImVec2 mousePos, float scale) const
	{
		float radius = m_SettingsContext.Get<TSE::Width>() * scale * 0.5f;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), radius + 1, s_OverlayHintColour, 50, s_OverlayHintThickness);
	}

}
