#pragma once

#include <fstream>

#include <glm/glm.hpp>

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 col;
};

// Indices (locations) of queue families (if they even exist)
struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentationFamily = -1;

	bool isValid()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};

struct SwapchainDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;			// surface properties, e.g image size
	std::vector<VkSurfaceFormatKHR> surfaceFormats;			// surface image formats, e.g R8G8B8A8
	std::vector<VkPresentModeKHR> presentationModes;		// presentation modes, eg. immediate mode, mailbox
};

struct SwapchainImage {
	VkImage image;
	VkImageView imageView;
};

static std::vector<char> readFile(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary | std::ios::ate);

	if (!file.is_open())
	{
		printf("Failed to open file %s\n", filepath.c_str());
		throw std::runtime_error("Failed to open file!");
	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> fileBuffer(fileSize);

	file.seekg(0);
	file.read(fileBuffer.data(), fileSize);
	file.close();
	return fileBuffer;
}