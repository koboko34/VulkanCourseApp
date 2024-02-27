#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Utilities.h"
#include "DebugUtilsMessenger.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

public:
	int init(GLFWwindow* window);
	void draw();
	void cleanup();

private:
	void createInstance();
	void createLogicalDevice();
	void createSurface();
	void createSwapchain();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSynchronisation();

	void recordCommands();

	void getPhysicalDevice();
	SwapchainDetails getSwapchainDetails(const VkPhysicalDevice& device) const;

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();

	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR>& presentationModes) const;
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities) const;

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	bool checkInstanceExtensionSupport(const std::vector<const char*>& extensionsToCheck) const;
	bool checkDeviceExtensionSupport(const VkPhysicalDevice& device) const;
	bool checkDeviceSuitable(const VkPhysicalDevice& device) const;
	bool checkValidationLayerSupport() const;

	QueueFamilyIndices getQueueFamilies(const VkPhysicalDevice& device) const;

private:
	GLFWwindow* window;

	int currentFrame = 0;

	VkInstance instance;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;
	std::vector<SwapchainImage> swapchainImages;
	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool graphicsCommandPool;
	VkDebugUtilsMessengerEXT debugMessenger;

	VkFormat swapchainFormat;
	VkExtent2D swapchainExtent;

	struct {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;

	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	std::vector<VkSemaphore> imageAvailable;
	std::vector<VkSemaphore> renderComplete;
	std::vector<VkFence> drawFences;
};
