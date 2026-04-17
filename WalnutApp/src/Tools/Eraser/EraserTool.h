#pragma once

#include "../Tool.h"
#include "../ToolSettingsEnum.h"
#include "../ToolSettingsProvider.h"

#include <memory>

namespace Stylus {

	class ComputeShader;
	using EraserSettingsContext = ToolSettingsProvider<TSE::Width, TSE::Shape, TSE::Antialiasing>;

	class EraserTool final : public Tool
	{
	public:
		EraserTool(std::function<void()> drawStrategy, ToolData toolData, std::shared_ptr<ComputeShader> shader, EraserSettingsContext context);

		void UseLeftClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
		void UseRightClick(glm::vec2 mousePos, glm::vec2 prevMousePos) const override;
	private:
		std::weak_ptr<ComputeShader> m_Shader;
		EraserSettingsContext m_SettingsContext;
	};

}
