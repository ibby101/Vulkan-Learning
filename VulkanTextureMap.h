#include "CommonHeaders.h"
#include "VulkanBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class VulkanTextureMap {
protected:
	VulkanBuffer vulkanBuffer;

	void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice);
};