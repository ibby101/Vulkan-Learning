#include "VulkanCubemap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void VulkanCubemap::createCubemap(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue) {
	
	int texWidth, texHeight, texChannels;

	stbi_uc* pixels = stbi_load(facePaths[0].c_str(), &texWidth,
								&texHeight, &texChannels, STBI_rgb_alpha);

	if (!pixels) {
		throw std::runtime_error("failed to load cubemap face texture.");
	}

	VkDeviceSize imageSize = texWidth * texHeight * 4;
	stbi_image_free(pixels);

	vulkanTMap.createImage(device, physicalDevice, texWidth, texHeight, 1, VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SAMPLE_COUNT_1_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
		cubemapImage, cubemapImageMemory);

	vulkanBuffer.transitionImageLayout(device, graphicsQueue, cubemapImage,
		VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, 6);

	for (uint32_t i = 0; i < 6; ++i) {
		pixels = stbi_load(facePaths[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

		if (!pixels) {
			throw std::runtime_error("failed to load cubemap face: " + facePaths[i]);
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		vulkanBuffer.createBuffer(device, physicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(device, stagingBufferMemory);

		stbi_image_free(pixels);

		vulkanBuffer.copyBufferToImage(device, graphicsQueue, stagingBuffer, cubemapImage, texWidth, texHeight, i);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
	}

	vulkanBuffer.transitionImageLayout(device, graphicsQueue, cubemapImage,
		VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, 6);
}

void VulkanCubemap::createCubemapImageView(VkDevice device) {
	vulkanTMap.createImageView(device, cubemapImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, 0, 6, VK_IMAGE_VIEW_TYPE_CUBE);
}

void VulkanCubemap::createCubemapSampler(VkDevice device, VkPhysicalDevice physicalDevice) {

}

