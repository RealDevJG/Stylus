#pragma once

#include "../UI/ICanvasView.h"
#include "../Systems/IShaderRegistryReadonly.h"
#include "../Systems/ToolSettingsStore.h"
#include "ToolActions.h"

namespace Stylus {

	class ToolActionExecutor
	{
	public:
		explicit ToolActionExecutor(const IShaderRegistryReadonly& shaderRegistryReadonly, const ICanvasView& canvasView, ToolSettingsStore& settingsStore);
		bool Execute(ToolAction& action) const;
	private:
		bool ExecuteComputeShader(const ComputeShaderAction& computeAction) const;
		bool ExecuteGraphicsShader(const GraphicsShaderAction& graphicsAction) const;
		bool UpdateSettingValue(const UpdateSettingValueAction& updateSettingAction) const;
	private:
		const IShaderRegistryReadonly& m_ShaderRegistryReadonly;
		const ICanvasView& m_ICanvasView;
		ToolSettingsStore& m_SettingsStore;
	};

}
