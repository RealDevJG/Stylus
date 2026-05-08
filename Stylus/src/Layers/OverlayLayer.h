#pragma once

#include <Walnut/Layer.h>
#include <memory>

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
