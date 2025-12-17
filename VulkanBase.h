#pragma once

#include "CommonHeaders.h"
#include "VulkanDebug.h"
#include "VulkanQueue.h"
#include "VulkanBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanUniformBuffer.h"
#include "VulkanTextureMap.h"
#include "VulkanModelLoader.h"

class VulkanBase {	
protected:

	VulkanBase()
		: vulkanTextureMap(vulkanBuffer),
		vulkanSwapChain(vulkanQueue, vulkanTextureMap, vulkanBuffer),
		vulkanUniformBuffer(vulkanBuffer, vulkanTextureMap)
	{}

	virtual ~VulkanBase() = default;

	VulkanDebug vulkanDebug;
	VulkanQueue vulkanQueue;
	VulkanBuffer vulkanBuffer;
	VulkanTextureMap vulkanTextureMap;
	VulkanSwapChain vulkanSwapChain;
	VulkanUniformBuffer vulkanUniformBuffer;
	VulkanModelLoader modelLoader;
	
	GLFWwindow* window = nullptr;
	VkInstance instance = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
	VkQueue transferQueue = VK_NULL_HANDLE;

	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	QueueFamilyIndices queueFamilies;
	float currentTime = (float)glfwGetTime();
	uint32_t currentFrame = 0;
	bool framebufferResized = false;

	virtual int rateDeviceSuitability(VkPhysicalDevice device) = 0;
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void setupDebugMessenger();
	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface(GLFWwindow* window);
	void createRenderPass();
	void createGraphicsPipeline();
	void createCommandPool(const QueueFamilyIndices& queueFamilies);
	void createVertexBuffer();
	void createIndexBuffer();
	void createCommandBuffer();
	void createDepthResources();
	void createFrameBuffers();
	void createDescriptorSetLayout();
	void createSwapSystem();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void createDescriptorComponents();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void createSyncObjects();
	void drawFrame();
	void createUniformBuffers();
	void recreateSwapChain();
	void loadModel();
	void cleanupSwapChain();
	void cleanupBuffers();
	void cleanupUniformBuffers();
	void cleanupTextureMapping();
	VkShaderModule createShaderModule(const std::vector<char>& code);	
};