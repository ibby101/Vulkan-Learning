#pragma once

#include "CommonHeaders.h"
#include "VulkanBuffer.h"

class VulkanTextureMap {
public:
	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VulkanTextureMap(VulkanBuffer& buffer);

	void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, 
		uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void createTextureImage(VkDevice device, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice);
	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void createTextureImageView(VkDevice device);
	void createTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);

	void generateMipMaps(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue,
		VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void cleanup(VkDevice device);

private:
	VulkanBuffer& vulkanBuffer;
};