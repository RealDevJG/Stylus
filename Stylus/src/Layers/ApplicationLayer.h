#pragma once

#include "../Systems/HistoryManager.h"

#include <Walnut/Application.h>
#include <Walnut/Core/Events/InputEvents.h>

#include <memory>

namespace Stylus {

	class ToolManager;
	class ToolRegistry;
	class ShaderRegistry;

	class ApplicationLayer final : public Walnut::Layer
	{
	public:
		ApplicationLayer(
			std::shared_ptr<ToolManager> toolManager,
			std::shared_ptr<ToolRegistry> toolRegistry,
			std::shared_ptr<ShaderRegistry> shaderRegistry
		) : m_ToolManager(toolManager), m_ToolRegistry(toolRegistry), m_ShaderRegistry(shaderRegistry) {}

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Walnut::Event& event) override;
	private:
		bool OnKeyPressed(Walnut::KeyPressedEvent& event);
	private:
		std::shared_ptr<ToolManager> m_ToolManager;
		std::shared_ptr<ToolRegistry> m_ToolRegistry;
		std::shared_ptr<ShaderRegistry> m_ShaderRegistry;
	};

}
