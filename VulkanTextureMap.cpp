#include "VulkanTextureMap.h"

void VulkanTextureMap::createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice) {
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(
		"textures/texture.jpg",
		&texWidth,
		&texHeight,
		&texChannels,
		STBI_rgb_alpha);

	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image.");
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	vulkanBuffer.createBuffer(device, physicalDevice, imageSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	stbi_image_free(pixels);
}