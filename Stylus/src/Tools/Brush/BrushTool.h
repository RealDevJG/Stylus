#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../../Systems/ToolSettingsProvider.h"

#include <memory>

namespace Stylus {

	class ComputeShader;
	using BrushSettingsContext = ToolSettingsProvider<TSE::PrimaryColour, TSE::SecondaryColour, TSE::Width, TSE::Shape, TSE::Antialiasing>;

	class BrushTool final : public Tool
	{
	public:
		BrushTool(std::function<void()> drawUiStrategy, ToolData toolData, std::shared_ptr<ComputeShader> shader, BrushSettingsContext context);

		bool UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		bool UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
	private:
		std::weak_ptr<ComputeShader> m_Shader;
		BrushSettingsContext m_SettingsContext;
	};

}
