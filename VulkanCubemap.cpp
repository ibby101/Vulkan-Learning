#include "VulkanCubemap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void VulkanCubemap::createImage(VkDevice device, VkPhysicalDevice physicalDevice,
	uint32_t width, uint32_t height, VkFormat format,
	VkImageTiling tiling, VkImageUsageFlags usage,
	VkMemoryPropertyFlags properties, VkImage& image,
	VkDeviceMemory& imageMemory) {

	VkImageCreateInfo imageInfo{};

	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 6; // for each face of the cubemap
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create cubemap image.");
	}
}