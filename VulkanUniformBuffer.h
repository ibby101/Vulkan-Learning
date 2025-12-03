#pragma once
#include "CommonHeaders.h"
#include "VulkanBuffer.h"

struct UniformBufferObject {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 project;
};

class VulkanUniformBuffer {
public:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	void createDescriptorSetLayout(VkDevice device);
	void createDescriptorPool(VkDevice device);
	void createDescriptorSets(VkDevice device);
	void updateUniformBuffer(VkDevice device, uint32_t currentImage, const VkExtent2D swapChainExtent);
};