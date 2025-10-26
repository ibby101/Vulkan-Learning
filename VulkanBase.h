#pragma once
#include "VulkanDebug.h"
#include "VulkanQueue.h"

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanBase : public VulkanDebug, public VulkanQueue {	
protected:
	VkInstance instance = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

	virtual int rateDeviceSuitability(VkPhysicalDevice device) = 0;
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface(GLFWwindow* window);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

};