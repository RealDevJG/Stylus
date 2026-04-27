#pragma once

#include <Walnut/Application.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <glm/glm.hpp>

namespace Stylus {

	class ToolManager;

	class OverlayLayer final : public Walnut::Layer
	{
	public:
		OverlayLayer(std::shared_ptr<ToolManager> toolManager)
			: m_ToolManager(toolManager) {}

		virtual void OnUIRender() override;
	private:
		std::shared_ptr<ToolManager> m_ToolManager;
	};

}
