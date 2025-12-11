#pragma once
#include "CommonHeaders.h"
#include "VulkanSwapChain.h"
#include "VulkanTextureMap.h"
#include "VulkanBuffer.h"

class VulkanDepthBuffer {
public:
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VulkanDepthBuffer(VulkanSwapChain& vulkanSwapChain, VulkanTextureMap& vulkanTMap, VulkanBuffer& vulkanBuffer);
	
	void createDepthResources(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
	
	VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

private:
	VulkanSwapChain& vulkanSwapChain;
	VulkanTextureMap& vulkanTMap;
	VulkanBuffer& vulkanBuffer;
};