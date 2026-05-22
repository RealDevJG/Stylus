#include "ShaderRegistry.h"

#include "../Vulkan/ShaderConfig.h"
#include <Walnut/Application.h>

namespace Stylus {

    ShaderRegistry::~ShaderRegistry()
    {
        Cleanup();
    }

    bool ShaderRegistry::RegisterCompute(const ComputeShaderEnum shaderEnum, const std::filesystem::path& computePath, uint32_t pushSize)
    {
        VkDevice device = Walnut::Application::GetDevice();

        if (m_DescriptorSetLayout == VK_NULL_HANDLE)
        {
            VkDescriptorSetLayoutBinding imageBinding{};
            imageBinding.binding = 0;
            imageBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            imageBinding.descriptorCount = 1;
            imageBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

            VkDescriptorSetLayoutCreateInfo layoutInfo{};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = 1;
            layoutInfo.pBindings = &imageBinding;

            if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
            {
                std::cerr << "Failed to create descriptor set layout in ShaderRegistry::RegisterCompute\n";
                return false;
            }
        }

        ShaderLayoutConfig layoutConfig;
        layoutConfig.DescriptorSetLayouts = { m_DescriptorSetLayout };
        layoutConfig.PushConstants = { { VK_SHADER_STAGE_COMPUTE_BIT, 0, pushSize } };

        auto computeShader = std::make_unique<ComputeShader>();
        if (computeShader->Init(device, computePath, layoutConfig))
        {
            m_ComputeShaders.try_emplace(shaderEnum, std::move(computeShader));
        }

        return true;
    }

    bool ShaderRegistry::RegisterGraphics(const GraphicsShaderEnum shaderEnum, const std::filesystem::path& vertPath, const std::filesystem::path& fragPath, uint32_t pushSize)
    {
        VkDevice device = Walnut::Application::GetDevice();

        // TODO: populate layoutConfig and renderPass fields
        ShaderLayoutConfig layoutConfig{};
        VkRenderPass renderPass{};

        auto graphicsShader = std::make_unique<GraphicsShader>();
        if (graphicsShader->Init(device, renderPass, vertPath, fragPath, layoutConfig))
        {
            m_GraphicsShaders.try_emplace(shaderEnum, std::move(graphicsShader));
        }

        return true;
    }

    void ShaderRegistry::UpdateStorageImage(VkImageView imageView)
    {
        VkDevice device = Walnut::Application::GetDevice();

        if (m_DescriptorPool == VK_NULL_HANDLE)
        {
            VkDescriptorPoolSize poolSize{};
            poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            poolSize.descriptorCount = 1;

            VkDescriptorPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            poolInfo.poolSizeCount = 1;
            poolInfo.pPoolSizes = &poolSize;
            poolInfo.maxSets = 1;

            if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
            {
                std::cerr << "Failed to create descriptor pool in ShaderRegistry::UpdateStorageImage\n";
                return;
            }

            VkDescriptorSetAllocateInfo allocateInfo{};
            allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            allocateInfo.descriptorPool = m_DescriptorPool;
            allocateInfo.descriptorSetCount = 1;
            allocateInfo.pSetLayouts = &m_DescriptorSetLayout;

            if (vkAllocateDescriptorSets(device, &allocateInfo, &m_DescriptorSet))
            {
                std::cerr << "Failed to create descriptor set in ShaderRegistry::UpdateStorageImage\n";
                return;
            }
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageView = imageView;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = m_DescriptorSet;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }

    ComputeShader* ShaderRegistry::GetCompute(const ComputeShaderEnum shaderEnum) const
    {
        auto it = m_ComputeShaders.find(shaderEnum);
        assert(it != m_ComputeShaders.end() && "Couldn't find shader in ShaderRegistry::GetCompute, did you forget to register it?");
        return it->second.get();
    }

    GraphicsShader* ShaderRegistry::GetGraphics(const GraphicsShaderEnum shaderEnum) const
    {
        auto it = m_GraphicsShaders.find(shaderEnum);
        assert(it != m_GraphicsShaders.end() && "Couldn't find shader in ShaderRegistry::GetGraphics, did you forget to register it?");
        return it->second.get();
    }

    VkDescriptorSet ShaderRegistry::GetDescriptorSet() const
    {
        return m_DescriptorSet;
    }

    void ShaderRegistry::Cleanup()
    {
        VkDevice device = Walnut::Application::GetDevice();
        m_ComputeShaders.clear();

        if (device != VK_NULL_HANDLE)
        {
            if (m_DescriptorPool != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorPool(device, m_DescriptorPool, nullptr);
                m_DescriptorPool = VK_NULL_HANDLE;
            }

            if (m_DescriptorSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(device, m_DescriptorSetLayout, nullptr);
                m_DescriptorSetLayout = VK_NULL_HANDLE;
            }
        }

        m_DescriptorSet = VK_NULL_HANDLE;
    }

}
