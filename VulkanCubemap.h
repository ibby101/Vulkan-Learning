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

	void createImage(VkDevice device, VkPhysicalDevice physicalDevice,
		uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image,
		VkDeviceMemory& imageMemory);

	void transitionImageLayout(VkDevice device, VkQueue graphicsQueue,
		VkImage image, VkFormat format, VkImageLayout oldLayout,
		VkImageLayout newLayout, uint32_t layerCount);

	void copyBufferToImage(VkDevice device, VkQueue graphicsQueue,
		VkBuffer buffer, VkImage image, uint32_t width,
		uint32_t height, uint32_t layer);


public:
	VulkanCubemap(VulkanBuffer& buffer, VulkanTextureMap& textureMap) : vulkanBuffer(buffer), vulkanTMap(textureMap) {}

	void createCubemap(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

	void createCubemapImageView(VkDevice device);

	void createCubemapSampler(VkDevice device, VkPhysicalDevice physicalDevice);

};