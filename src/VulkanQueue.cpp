#include "VulkanQueue.h"

QueueFamilyIndices VulkanQueue::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
	QueueFamilyIndices indices;


	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;	
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if ((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
			!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
			!(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
			indices.transferFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		++i;
	}

	if (!indices.transferFamily.has_value() && indices.graphicsFamily.has_value()) {
		indices.transferFamily = indices.graphicsFamily;
	}

	return indices;
}