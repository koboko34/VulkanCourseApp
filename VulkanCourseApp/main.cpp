#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <stdexcept>

#include "VulkanRenderer.h"

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

void initWindow(const std::string& windowName = "Test Window", const int width = 800, const int height = 600)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

int main()
{
	initWindow();

	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	vulkanRenderer.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}