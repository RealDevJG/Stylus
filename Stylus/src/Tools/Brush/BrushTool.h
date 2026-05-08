#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../../Systems/ToolSettingsProvider.h"

namespace Stylus {

	class ComputeShader;
	using BrushSettingsContext = ToolSettingsProvider<TSE::PrimaryColour, TSE::SecondaryColour, TSE::Width, TSE::Shape, TSE::Antialiasing>;

	class BrushTool final : public Tool
	{
	public:
		BrushTool(std::function<void()> drawUiStrategy, const ToolData& toolData, const ComputeShader* shader, BrushSettingsContext context);

		bool UseLeftClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		bool UseRightClick(const glm::vec2 mousePos, const glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(const ImVec2 mousePos, float scale) const override;
	private:
		const ComputeShader* m_Shader;
		BrushSettingsContext m_SettingsContext;
	};

}
