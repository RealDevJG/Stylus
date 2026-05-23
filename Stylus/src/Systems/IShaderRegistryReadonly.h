#pragma once

#include "../Vulkan/ComputeShader.h"
#include "../Vulkan/GraphicsShader.h"
#include "../Vulkan/ShaderEnums.h"

#include <vulkan/vulkan.h>

namespace Stylus {

	class IShaderRegistryReadonly
	{
	public:
		virtual ~IShaderRegistryReadonly() = default;

		[[nodiscard]] virtual ComputeShader* GetCompute(const ComputeShaderEnum shaderEnum) const = 0;
		[[nodiscard]] virtual GraphicsShader* GetGraphics(const GraphicsShaderEnum shaderEnum) const = 0;
		[[nodiscard]] virtual VkDescriptorSet GetDescriptorSet() const = 0;
		[[nodiscard]] virtual VkFramebuffer GetFramebuffer() const = 0;
		[[nodiscard]] virtual VkRenderPass GetRenderPass() const = 0;
	};

}
