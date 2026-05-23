#pragma once

#include "../Serialisation/FileReader.h"

#include <vulkan/vulkan.h>

#include <expected>
#include <filesystem>
#include <iostream>
#include <vector>

namespace Stylus {

	struct ShaderLayoutConfig
	{
		std::vector<VkDescriptorSetLayout> DescriptorSetLayouts;
		std::vector<VkPushConstantRange> PushConstants;
	};

	inline std::vector<uint32_t> ReadShaderFile(const std::filesystem::path& filePath)
	{
		FileReader file{ filePath };

		if (auto spirvData = file.ReadSpirv(); spirvData.has_value())
		{
			return *spirvData;
		}
		else if (spirvData.error() == FileError::FileNotGood)
		{
			std::cerr << "Shader " << filePath << " .good() failed; is it the correct path?\n";
		}
		else if (spirvData.error() == FileError::FileInvalidSPIRV)
		{
			std::cerr << "Shader " << filePath << " is invalid SPIR-V\n";
		}

		return {};
	}

	inline VkShaderModule CreateShaderModule(VkDevice device, const std::vector<uint32_t>& code)
	{
		if (code.empty())
		{
			std::cerr << "The code provided to CreateShaderModule is empty\n";
			return VK_NULL_HANDLE;
		}

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size() * sizeof(uint32_t);
		createInfo.pCode = code.data();

		VkShaderModule shaderModule = VK_NULL_HANDLE;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			std::cerr << "Failed to create shader module in CreateShaderModule\n";
			return VK_NULL_HANDLE;
		}

		return shaderModule;
	}

}
