#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../../Systems/ToolSettingsProvider.h"

namespace Stylus {

	class ComputeShader;
	using EraserSettingsContext = ToolSettingsProvider<TSE::Width, TSE::Shape, TSE::Antialiasing>;

	class EraserTool final : public Tool
	{
	public:
		EraserTool(std::function<void()> drawUiStrategy, const ToolData& toolData, const ComputeShader* shader, EraserSettingsContext context);

		bool UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		bool UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(const ImVec2 mousePos, float scale) const override;
	private:
		const ComputeShader* m_Shader;
		EraserSettingsContext m_SettingsContext;
	};

}
