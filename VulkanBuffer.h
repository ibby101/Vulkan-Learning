#pragma once
#include "CommonHeaders.h"
#include "VulkanQueue.h"

class VulkanBuffer {
public:
	VkCommandPool commandPool;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// Helper function to find suitable memory type
	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	// Creation of buffer
	void createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void createCommandPool(VkDevice device, const QueueFamilyIndices& indices);

	void createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue submitQueue, const std::vector<Vertex> vertices);

	void createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue submitQueue, const std::vector<uint16_t> indices);

	// Helper to copy data from one buffer to another using a command buffer
	void copyBuffer(VkDevice device, VkQueue graphicsQueue,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void copyBufferToImage(VkDevice device, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	VkCommandBuffer beginSingleTimeCommands(VkDevice device);

	void endSingleTimeCommands(VkDevice device, VkQueue submitQueue, VkCommandBuffer commandBuffer);

	void transitionImageLayout(VkDevice device, VkQueue submitQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	bool hasStencilComponent(VkFormat format);

	void cleanup(VkDevice device);
};