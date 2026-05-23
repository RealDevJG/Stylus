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

        if (m_RenderPass == VK_NULL_HANDLE)
        {
            VkAttachmentDescription colourAttachment{};
            colourAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
            colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colourAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkAttachmentReference colourAttachmentRef{};
            colourAttachmentRef.attachment = 0;
            colourAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass{};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colourAttachmentRef;

            VkRenderPassCreateInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = 1;
            renderPassInfo.pAttachments = &colourAttachment;
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;

            if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
            {
                std::cerr << "Failed to create render pass in ShaderRegistry::RegisterGraphics\n";
                return false;
            }
        }

        ShaderLayoutConfig layoutConfig{};
        layoutConfig.PushConstants = { { VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, pushSize } };
        layoutConfig.DescriptorSetLayouts = { m_DescriptorSetLayout };

        auto graphicsShader = std::make_unique<GraphicsShader>();
        if (graphicsShader->Init(device, m_RenderPass, vertPath, fragPath, layoutConfig))
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

    void ShaderRegistry::UpdateFramebuffers(VkDevice device, VkImageView imageView, uint32_t width, uint32_t height)
    {
        if (m_Framebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(device, m_Framebuffer, nullptr);
            m_Framebuffer = VK_NULL_HANDLE;
        }

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = m_RenderPass;
        framebufferInfo.attachmentCount = 1;

        VkImageView attachments[] = { imageView };
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_Framebuffer) != VK_SUCCESS)
        {
            std::cerr << "Failed to create frame buffer in ShaderRegistry::UpdateFramebuffers\n";
            return;
        }
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

    VkFramebuffer ShaderRegistry::GetFramebuffer() const
    {
        return m_Framebuffer;
    }

    VkRenderPass ShaderRegistry::GetRenderPass() const
    {
        return m_RenderPass;
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

            if (m_Framebuffer != VK_NULL_HANDLE)
            {
                vkDestroyFramebuffer(device, m_Framebuffer, nullptr);
                m_Framebuffer = VK_NULL_HANDLE;
            }

            if (m_RenderPass != VK_NULL_HANDLE)
            {
                vkDestroyRenderPass(device, m_RenderPass, nullptr);
                m_RenderPass = VK_NULL_HANDLE;
            }
        }

        m_DescriptorSet = VK_NULL_HANDLE;
    }

}
