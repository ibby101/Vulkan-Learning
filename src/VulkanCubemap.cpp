#include "VulkanCubemap.h"

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
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 6,
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
	cubemapImageView = vulkanTMap.createImageView(device, cubemapImage, VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT, 1, 6, VK_IMAGE_VIEW_TYPE_CUBE);
}

void VulkanCubemap::createCubemapSampler(VkDevice device, VkPhysicalDevice physicalDevice) {

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(physicalDevice, &properties);

	VkSamplerCreateInfo samplerInfo{};

	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	if (vkCreateSampler(device, &samplerInfo, nullptr, &cubemapSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create cubemap sampler.");
	}
}

void VulkanCubemap::createSkyboxDescriptorSetLayout(VkDevice device) {

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;


	std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
		uboLayoutBinding, samplerLayoutBinding
	};

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr,
		&skyboxDescriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create skybox descriptor set layout");
	}
}

void VulkanCubemap::createSkyboxVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue) {
	VkDeviceSize bufferSize = sizeof(float) * skyboxVertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	vulkanBuffer.createBuffer(device, physicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, skyboxVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	vulkanBuffer.createBuffer(device, physicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		skyboxVertexBuffer, skyboxVertexBufferMemory);

	vulkanBuffer.copyBuffer(device, graphicsQueue, stagingBuffer,
		skyboxVertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void VulkanCubemap::createSkyboxDescriptorSets(VkDevice device) {
	skyboxDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT,
		skyboxDescriptorSetLayout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = vulkanUniform.descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(device, &allocInfo, skyboxDescriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate skybox descriptor sets.");
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = vulkanUniform.uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = getCubemapImageView();
		imageInfo.sampler = getCubemapSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = skyboxDescriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;


		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = skyboxDescriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()),
			descriptorWrites.data(), 0, nullptr);
	}
}

void VulkanCubemap::cleanup(VkDevice device) {
	vkDestroySampler(device, cubemapSampler, nullptr);
	vkDestroyImageView(device, cubemapImageView, nullptr);
	vkDestroyImage(device, cubemapImage, nullptr);
	vkFreeMemory(device, cubemapImageMemory, nullptr);
}

