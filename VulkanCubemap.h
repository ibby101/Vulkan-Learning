#pragma once

#include "CommonHeaders.h"
#include "VulkanBuffer.h"
#include "VulkanTextureMap.h"

class VulkanCubemap {
private:
	VulkanBuffer& vulkanBuffer;
	VulkanTextureMap& vulkanTMap;

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
	VulkanCubemap(VulkanBuffer& buffer, VulkanTextureMap& textureMap) : vulkanBuffer(buffer), vulkanTMap(textureMap) {}

	void createCubemap(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

	void createCubemapImageView(VkDevice device);

	void createCubemapSampler(VkDevice device, VkPhysicalDevice physicalDevice);

	void cleanup(VkDevice device);

};