#pragma once

#include "CommonHeaders.h"
#include "VulkanQueue.h"
#include "VulkanBuffer.h"
#include "VulkanTextureMap.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapChain {
public:
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VulkanSwapChain(VulkanQueue& queue, VulkanTextureMap& tMap, VulkanBuffer& buffer) : vulkanQueue(queue), vulkanTMap(tMap), vulkanBuffer(buffer) {}

	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	void createSwapSystem(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices indices);

	void createFramebuffers(VkDevice device, VkRenderPass renderPass, VkImageView depthImageView);

	void createImageViews(VkDevice device);

	void recreate(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface,
		VkQueue graphicsQueue, GLFWwindow* window, VkRenderPass renderPass, QueueFamilyIndices indices);

	void cleanup(VkDevice device);

	void createDepthResources(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkExtent2D swapChainExtent);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);

	VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

private:
	VulkanQueue& vulkanQueue;
	VulkanTextureMap& vulkanTMap;
	VulkanBuffer& vulkanBuffer;

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};

