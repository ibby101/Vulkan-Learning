#pragma once
#include "VulkanDebug.h"
#include "VulkanQueue.h"

class VulkanBase : public VulkanDebug, public VulkanQueue {
protected:
	VkInstance instance = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	virtual int rateDeviceSuitability(VkPhysicalDevice device) = 0;
	bool isDeviceSuitable(VkPhysicalDevice device);

	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
};