#pragma once
#include "CommonHeaders.h"
#include "VulkanBuffer.h"

class VulkanUniformBuffer {
public:

	VulkanBuffer& vulkanBuffer;

	VulkanUniformBuffer(VulkanBuffer& vulkanBuffer) : vulkanBuffer(vulkanBuffer) {}

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	void createUniformBuffer(VkDevice device, VkPhysicalDevice physicalDevice);
	void updateUniformBuffer(VkDevice device, uint32_t currentImage, const VkExtent2D swapChainExtent);
	void createDescriptorSetLayout(VkDevice device);
	void createDescriptorPool(VkDevice device);
	void createDescriptorSets(VkDevice device);
	
	void cleanup(VkDevice device);
};