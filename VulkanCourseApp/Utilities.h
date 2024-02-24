#pragma once

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
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