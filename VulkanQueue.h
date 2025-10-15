#pragma once
#include "everything.h"

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;

	bool isComplete() {
		return graphicsFamily.has_value();
	}
};

class VulkanQueue {
protected:
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};