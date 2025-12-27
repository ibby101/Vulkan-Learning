#pragma once

#include "CommonHeaders.h"
#include "VulkanBuffer.h"
#include "VulkanTextureMap.h"
#include "VulkanUniformBuffer.h"

class VulkanCubemap {
private:
	VulkanBuffer& vulkanBuffer;
	VulkanTextureMap& vulkanTMap;
	VulkanUniformBuffer& vulkanUniform;

	VkImage cubemapImage = VK_NULL_HANDLE;
	VkDeviceMemory cubemapImageMemory = VK_NULL_HANDLE;
	VkImageView cubemapImageView = VK_NULL_HANDLE;
	VkSampler cubemapSampler = VK_NULL_HANDLE;

	std::array<std::string, 6> facePaths = {
		"textures/skybox/right.jpg",
		"textures/skybox/left.jpg",
		"textures/skybox/top.jpg",
		"textures/skybox/bottom.jpg",
		"textures/skybox/front.jpg",
		"textures/skybox/back.jpg"
	};


public:
	VkPipeline skyboxPipeline = VK_NULL_HANDLE;
	VkPipelineLayout skyboxPipelineLayout = VK_NULL_HANDLE;
	VkDescriptorSetLayout skyboxDescriptorSetLayout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSet> skyboxDescriptorSets;

	VkBuffer skyboxVertexBuffer;
	VkDeviceMemory skyboxVertexBufferMemory;

    // Skybox vertex data (simple cube)
    std::vector<float> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };



	VulkanCubemap(VulkanBuffer& buffer, VulkanTextureMap& textureMap, VulkanUniformBuffer& uBuffer) : vulkanBuffer(buffer), vulkanTMap(textureMap), vulkanUniform(uBuffer) {
    
        // scaling all vertices by 10
        float scale = 500.0f;
        for (float& vertex : skyboxVertices) {
            vertex *= scale;
        }
    }

	void createCubemap(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
	void createCubemapImageView(VkDevice device);
	void createCubemapSampler(VkDevice device, VkPhysicalDevice physicalDevice);

	void createSkyboxVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);
	void createSkyboxDescriptorSetLayout(VkDevice device);
	void createSkyboxDescriptorSets(VkDevice device);

	VkImageView getCubemapImageView() const { return cubemapImageView; }
	VkSampler getCubemapSampler() const { return cubemapSampler; }

	void cleanup(VkDevice device);
};