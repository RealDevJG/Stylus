#pragma once

#include <glm/vec2.hpp>
#include <imgui.h>

#include <array>

namespace Stylus::Utils {

	inline static constexpr size_t g_MaxPushConstantSize = 128;

	inline glm::vec2 ToGlmVec2(const ImVec2& v)
	{
		return glm::vec2{ v.x, v.y };
	}

	inline ImVec2 ToImVec2(const glm::vec2& v)
	{
		return ImVec2{ v.x, v.y };
	}

	template<typename T> requires std::is_trivially_copyable_v<T>
	[[nodiscard]] constexpr auto CopyToBytes(const T& structure)
	{
		static_assert(sizeof(T) <= g_MaxPushConstantSize);

		std::array<std::byte, g_MaxPushConstantSize> byteBuffer;
		std::memcpy(byteBuffer.data(), &structure, sizeof(T));
		return byteBuffer;
	}

}
