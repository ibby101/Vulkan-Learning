#include "VulkanBase.h"

class HelloTriangleApplication : public VulkanBase {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
private:		
	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(WIDTH, HEIGHT, "Hmm, Yeess", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void initVulkan() {
		createInstance();
		vulkanDebug.setupDebugMessenger(instance);
		createSurface(window);
		pickPhysicalDevice();
		createLogicalDevice();
		QueueFamilyIndices queueFamilies = vulkanQueue.findQueueFamilies(physicalDevice, surface);
		vulkanSwapChain.createSwapSystem(device, physicalDevice, surface, window, queueFamilies);
		createRenderPass();
		vulkanUniformBuffer.createDescriptorSetLayout(device);
		createGraphicsPipeline();
		vulkanSwapChain.createFramebuffers(device, renderPass);
		createCommandPool();
		createVertexBuffer();
		createIndexBuffer();
		createUniformBuffers();
		createCommandBuffer();
		createSyncObjects();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(device);
	}

	void cleanup() {
		cleanupSwapChain();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			vkDestroyBuffer(device, vulkanUniformBuffer.uniformBuffers[i], nullptr);
			vkFreeMemory(device, vulkanUniformBuffer.uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorSetLayout(device, vulkanUniformBuffer.descriptorSetLayout, nullptr);

		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);

		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
		
		vkDestroyPipeline(device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);


		vkDestroyRenderPass(device, renderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
			vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
			vkDestroyFence(device, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(device, commandPool, nullptr);

		vkDestroyDevice(device, nullptr);


		if (enableValidationLayers) {
			vulkanDebug.DestroyDebugUtilsMessengerEXT(instance, vulkanDebug.debugMessenger, nullptr);
		}

		// surface and instance cleanup
		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		// glfw cleanup
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	int rateDeviceSuitability(VkPhysicalDevice device) override {
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		int score = 0;

		// ensuring discrete GPUs have a significant advantage
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += 1000;
		}
		// maximum size of textures for graphics
		score += deviceProperties.limits.maxImageDimension2D;

		if (!deviceFeatures.geometryShader) {
			return 0;
		}

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;

		// checking for swap chain support only if extensions are supported
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = vulkanSwapChain.querySwapChainSupport(device, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
	
		
		// checking if the device has the required queue families
		QueueFamilyIndices indices = vulkanQueue.findQueueFamilies(device, surface);

		if (!indices.isComplete() || !swapChainAdequate) {
			return 0;
		}

		return score;
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}