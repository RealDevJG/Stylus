#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../../Systems/ToolSettingsProvider.h"

#include <memory>

namespace Stylus {

	class ComputeShader;
	using EraserSettingsContext = ToolSettingsProvider<TSE::Width, TSE::Shape, TSE::Antialiasing>;

	class EraserTool final : public Tool
	{
	public:
		EraserTool(std::function<void()> drawUiStrategy, ToolData toolData, std::shared_ptr<ComputeShader> shader, EraserSettingsContext context);

		bool UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		bool UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		void DrawOverlayHint(ImVec2 mousePos, float scale) const override;
	private:
		std::weak_ptr<ComputeShader> m_Shader;
		EraserSettingsContext m_SettingsContext;
	};

}
