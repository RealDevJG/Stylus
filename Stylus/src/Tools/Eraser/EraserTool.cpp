#include "EraserTool.h"

#include "EraserPushData.h"
#include "../../Vulkan/ComputeShader.h"

namespace Stylus {

	EraserTool::EraserTool(std::function<void()> drawUiStrategy, ToolData toolData, std::shared_ptr<ComputeShader> shader, EraserSettingsContext context)
		: Tool(drawUiStrategy, toolData), m_Shader(shader), m_SettingsContext(context) {}

	bool EraserTool::UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
	{
		if (auto shader = m_Shader.lock())
		{
			EraserPushData pushData{
				mousePos,
				prevMousePos,
				static_cast<int>(m_SettingsContext.Get<TSE::Shape>()),
				m_SettingsContext.Get<TSE::Width>(),
				m_SettingsContext.Get<TSE::Antialiasing>() ? 1u : 0u
			};

			shader->DispatchShader(&pushData);
			return true;
		}

		return false;
	}

	bool EraserTool::UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const
	{
		if (auto shader = m_Shader.lock())
		{
			EraserPushData pushData{
				mousePos,
				prevMousePos,
				static_cast<int>(m_SettingsContext.Get<TSE::Shape>()),
				m_SettingsContext.Get<TSE::Width>(),
				m_SettingsContext.Get<TSE::Antialiasing>() ? 1u : 0u
			};

			shader->DispatchShader(&pushData);
			return true;
		}

		return false;
	}

	void EraserTool::DrawOverlayHint(ImVec2 mousePos) const
	{
		float radius = m_SettingsContext.Get<TSE::Width>();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), radius + 1, s_OverlayHintColour, 50, s_OverlayHintThickness);
	}

}
