#pragma once
#include "CommonHeaders.h"
#include "VulkanBuffer.h"

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 project;
};

class VulkanUniformBuffer {
public:
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorSetLayout descriptorSetLayout;

	void createDescriptorSetLayout(VkDevice device);
	void updateUniformBuffer(VkDevice device, uint32_t currentImage, float time, const VkExtent2D swapChainExtent);
};