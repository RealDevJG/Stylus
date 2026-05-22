#pragma once

#include "../Systems/IToolSettingsUpdater.h"
#include "../Utils/ConversionUtils.h"
#include "../Vulkan/ShaderEnums.h"

#include <array>
#include <cstdint>
#include <variant>
#include <memory>

namespace Stylus {

	enum class ToolActionType : uint8_t
	{
		ComputeShaderAction = 0,
		GraphicsShaderAction,
		UpdateSettingValueAction
	};

	struct ComputeShaderAction
	{
		ComputeShaderEnum ShaderEnum{};
		std::array<std::byte, Utils::g_MaxPushConstantSize> PushData;
		uint32_t PushDataSize{};
	};

	struct GraphicsShaderAction
	{
		GraphicsShaderEnum ShaderEnum{};
		std::array<std::byte, Utils::g_MaxPushConstantSize> PushData;
		uint32_t PushDataSize{};
	};

	struct UpdateSettingValueAction
	{
		std::unique_ptr<IToolSettingsUpdater> SettingsUpdater;
	};

	using ToolActionData = std::variant<
		std::monostate,
		ComputeShaderAction,
		GraphicsShaderAction,
		UpdateSettingValueAction
	>;

	struct ToolAction
	{
		ToolActionType Type;
		ToolActionData Data;
	};

}
