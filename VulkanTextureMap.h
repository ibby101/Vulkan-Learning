#include "CommonHeaders.h"
#include "VulkanBuffer.h"

class VulkanTextureMap {
public:
	VulkanBuffer vulkanBuffer;

	void createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width,
		uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory);

	void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice);

	
};