#include "BrushTool.h"

#include "../../Vulkan/PushData.h"
#include "../ToolSettingsEnum.h"

#include <imgui.h>

namespace Stylus {

	BrushTool::BrushTool(const ToolData& toolData, std::function<void()> drawSettingsUIStrategy, ToolSettingsStore& settingsStore)
		: Tool(toolData, drawSettingsUIStrategy), m_SettingsStore(settingsStore) {}

	ToolAction BrushTool::GetLeftClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
		BrushLikePushData pushData{
			m_SettingsStore.GetValue<TSE::PrimaryColour>(),
			mousePos,
			prevMousePos,
			static_cast<int>(m_SettingsStore.GetValue<TSE::Shape>()),
			m_SettingsStore.GetValue<TSE::Width>(),
			m_SettingsStore.GetValue<TSE::Antialiasing>() ? 1u : 0u
		};

		return ToolAction{
			ToolActionType::ComputeShaderAction,
			ComputeShaderAction{
				.ShaderEnum = ComputeShaderEnum::BrushLikeTool,
				.PushData = Utils::CopyToBytes(pushData),
				.PushDataSize = GetToolData().ComputePushConstantSize
			}
		};
	}

	ToolAction BrushTool::GetRightClickAction(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const
	{
		BrushLikePushData pushData{
			m_SettingsStore.GetValue<TSE::SecondaryColour>(),
			mousePos,
			prevMousePos,
			static_cast<int>(m_SettingsStore.GetValue<TSE::Shape>()),
			m_SettingsStore.GetValue<TSE::Width>(),
			m_SettingsStore.GetValue<TSE::Antialiasing>() ? 1u : 0u
		};

		return ToolAction{
			ToolActionType::ComputeShaderAction,
			ComputeShaderAction{
				.ShaderEnum = ComputeShaderEnum::BrushLikeTool,
				.PushData = Utils::CopyToBytes(pushData),
				.PushDataSize = GetToolData().ComputePushConstantSize
			}
		};
	}

	void BrushTool::DrawOverlayHint(const glm::vec2 mousePos, float scale) const
	{
		float radius = m_SettingsStore.GetValue<TSE::Width>() * scale * 0.5f;

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), radius + 1, IM_COL32(0, 0, 0, 255), 50, s_OverlayHintThickness);
	}

}
