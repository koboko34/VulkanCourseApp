#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, const std::vector<Vertex>* vertices);
	~Mesh();

	void destroyVertexBuffer();

	uint32_t getVertexCount() const { return vertexCount; }
	VkBuffer getVertexBuffer() const { return vertexBuffer; }
private:
	uint32_t vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;

	void createVertexBuffer(const std::vector<Vertex>* vertices);

	uint32_t findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties);
};

