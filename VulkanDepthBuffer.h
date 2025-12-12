#pragma once
#include "CommonHeaders.h"
#include "VulkanTextureMap.h"
#include "VulkanBuffer.h"

class VulkanDepthBuffer {
public:
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VulkanDepthBuffer(VulkanTextureMap& vulkanTMap, VulkanBuffer& vulkanBuffer);
	
	void createDepthResources(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkExtent2D swapChainExtent);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
	
	VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

private:
	VulkanTextureMap& vulkanTMap;
	VulkanBuffer& vulkanBuffer;
};