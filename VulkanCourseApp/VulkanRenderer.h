#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Utilities.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

public:
	int init(GLFWwindow* window);
	void cleanup();

private:
	void createInstance();

	void getPhysicalDevice();
	void createLogicalDevice();

	bool checkInstanceExtensionSupport(const std::vector<const char*>& extensionsToCheck) const;
	bool checkDeviceSuitable(const VkPhysicalDevice& device) const;

	QueueFamilyIndices getQueueFamilies(const VkPhysicalDevice& device) const;

private:
	GLFWwindow* window;

	VkInstance instance;

	struct {
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;

	VkQueue graphicsQueue;
};

