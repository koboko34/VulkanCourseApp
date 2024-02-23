#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::~VulkanRenderer()
{
}

int VulkanRenderer::init(GLFWwindow* window)
{
	this->window = window;
	try
	{
		createInstance();
		getPhysicalDevice();
		createLogicalDevice();
	}
	catch (const std::runtime_error& e)
	{
		printf("ERROR: %s", e.what());
		return EXIT_FAILURE;
	}

	return 0;
}

void VulkanRenderer::cleanup()
{
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void VulkanRenderer::createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.pEngineName = "No engine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;
	
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
	std::vector<const char*> instanceExtensions;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	if (!checkInstanceExtensionSupport(instanceExtensions))
	{
		throw std::runtime_error("VkInstance does not support required extensions!");
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// todo: setup validation layers that the instance will use
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan Instance!");
	}
}

void VulkanRenderer::getPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Can't find a GPU which supports the Vulkan Instance!");
	}

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

	for (const VkPhysicalDevice& device : physicalDevices)
	{
		if (checkDeviceSuitable(device))
		{
			mainDevice.physicalDevice = device;
			return;
		}
	}
}

void VulkanRenderer::createLogicalDevice()
{
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);
	
	// Queues the logical device needs to create and the info to do so (only 1 for now)
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;
	float priority = 1.f;
	queueCreateInfo.pQueuePriorities = &priority;
	
	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.enabledExtensionCount = 0;
	createInfo.ppEnabledExtensionNames = nullptr;
	createInfo.pEnabledFeatures = &deviceFeatures;

	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &createInfo, nullptr, &mainDevice.logicalDevice);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
}

bool VulkanRenderer::checkInstanceExtensionSupport(const std::vector<const char*>& extensionsToCheck) const
{
	// first, get number of extensions so we know what size to set for our vector
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	for (const char* extensionToCheck : extensionsToCheck)
	{
		bool hasExtension = false;
		for (const VkExtensionProperties& extension : extensions)
		{
			if (strcmp(extensionToCheck, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension)
		{
			return false;
		}
	}
	return true;
}

bool VulkanRenderer::checkDeviceSuitable(const VkPhysicalDevice& device) const
{
	QueueFamilyIndices indices = getQueueFamilies(device);
	return indices.isValid();
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(const VkPhysicalDevice& device) const
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		if (indices.isValid())
		{
			break;
		}
		i++;
	}
	return indices;
}
