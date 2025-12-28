#pragma once
#include "CommonHeaders.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	std::optional<uint32_t> transferFamily;

	bool isComplete() {
		return graphicsFamily.has_value() &&
			presentFamily.has_value() &&
			transferFamily.has_value();
	}
};

class VulkanQueue {
public:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
};