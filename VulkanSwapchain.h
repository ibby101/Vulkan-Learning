#pragma once

#include "CommonHeaders.h"
#include "VulkanQueue.h"
#include "VulkanTextureMap.h"

// forward declaration of VulkanTextureMap



struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapChain {
public:
	VulkanQueue vulkanQueue;
	VulkanTextureMap& vulkanTMap;

	VulkanSwapChain(VulkanTextureMap& tMap) : vulkanTMap(tMap) {}

	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	void createSwapSystem(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices indices);
	void createFramebuffers(VkDevice device, VkRenderPass renderPass);
	void createImageViews(VkDevice device);
	void recreate(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, VkRenderPass renderPass, QueueFamilyIndices indices);
	void cleanup(VkDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};

