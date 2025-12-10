#pragma once

#include "CommonHeaders.h"
#include "VulkanBuffer.h"

class VulkanTextureMap {
public:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VulkanTextureMap(VulkanBuffer& buffer);

	void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width,
		uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory);

	void createTextureImage(VkDevice device, VkQueue submitQueue, VkPhysicalDevice physicalDevice);
	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void createTextureImageView(VkDevice device);
	void createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);

	void cleanup(VkDevice device);

private:
	VulkanBuffer& vulkanBuffer;
};