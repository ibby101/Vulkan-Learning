#pragma once

#include "CommonHeaders.h"
#include "VulkanTextureMap.h"

class VulkanSampling {
public:

	VulkanSampling(VulkanTextureMap& tmap) : vulkanTM(tmap) {}

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;


	VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);

	void createColorResources(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D swapChainExtent, VkFormat format);


private:
	VulkanTextureMap& vulkanTM;
};
