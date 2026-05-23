#include "BrushTool.h"

#include "../../Vulkan/PushData.h"
#include "../ToolSettingsEnum.h"

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

	ToolAction BrushTool::DrawOverlayHint(const glm::vec2 mousePos, float scale) const
	{
		BrushLikeOverlayPushData pushData{
			.MousePos = mousePos,
			.Width = m_SettingsStore.GetValue<TSE::Width>(),
			.Scale = scale,
			.Shape = static_cast<int>(m_SettingsStore.GetValue<TSE::Shape>())
		};

		return ToolAction{
			ToolActionType::GraphicsShaderAction,
			GraphicsShaderAction{
				.ShaderEnum = GraphicsShaderEnum::BrushLikeToolOverlay,
				.PushData = Utils::CopyToBytes(pushData),
				.PushDataSize = GetToolData().GraphicsPushConstantSize
			}
		};
	}

}
