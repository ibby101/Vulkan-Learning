#pragma once
#include "CommonHeaders.h"
#include "VulkanQueue.h"

class VulkanBuffer {
public:
	VkCommandPool commandPool;
	VkCommandPool transferCommandPool;

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

	void createTransferCommandPool(VkDevice device, const QueueFamilyIndices& indices);

	void createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, const std::vector<Vertex> vertices);

	void createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, const std::vector<uint32_t> indices);

	// Helper to copy data from one buffer to another using a command buffer
	void copyBuffer(VkDevice device, VkQueue graphicsQueue,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void copyBufferToImage(VkDevice device, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	VkCommandBuffer beginSingleTimeCommands(VkDevice device);

	void endSingleTimeCommands(VkDevice device, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);

	void transitionImageLayout(VkDevice device, VkQueue graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	bool hasStencilComponent(VkFormat format);

	void cleanup(VkDevice device);
};