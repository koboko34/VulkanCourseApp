#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vector>

#include "Utilities.h"

class Mesh
{
public:
	Mesh();
	Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkQueue transferQueue, VkCommandPool transferCommandPool,
		const std::vector<Vertex>* vertices, const std::vector<uint32_t>* indices);
	~Mesh();

	void destroyBuffers();

	uint32_t getVertexCount() const { return vertexCount; }
	VkBuffer getVertexBuffer() const { return vertexBuffer; }
	uint32_t getIndexCount() const { return indexCount; }
	VkBuffer getIndexBuffer() const { return indexBuffer; }
private:
	uint32_t vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	uint32_t indexCount;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;

	void createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, const std::vector<Vertex>* vertices);
	void createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, const std::vector<uint32_t>* indices);
};

