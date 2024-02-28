#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, const std::vector<Vertex>* vertices)
{
	this->physicalDevice = physicalDevice;
	this->logicalDevice = logicalDevice;
	vertexCount = static_cast<uint32_t>(vertices->size());
	createVertexBuffer(vertices);
}

Mesh::~Mesh()
{
}

void Mesh::destroyVertexBuffer()
{
	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
}

void Mesh::createVertexBuffer(const std::vector<Vertex>* vertices)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeof(Vertex) * vertices->size();
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	
	VkResult result = vkCreateBuffer(logicalDevice, &bufferCreateInfo, nullptr, &vertexBuffer);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create vertex buffer!");
	}

	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements(logicalDevice, vertexBuffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryTypeIndex(memoryRequirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	result = vkAllocateMemory(logicalDevice, &memoryAllocateInfo, nullptr, &vertexBufferMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate memory for vertex buffer!");
	}

	result = vkBindBufferMemory(logicalDevice, vertexBuffer, vertexBufferMemory, 0);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to bind vertex buffer memory!");
	}

	void* data;
	vkMapMemory(logicalDevice, vertexBufferMemory, 0, bufferCreateInfo.size, 0, &data);
	memcpy(data, vertices->data(), static_cast<size_t>(bufferCreateInfo.size));
	vkUnmapMemory(logicalDevice, vertexBufferMemory);
}

uint32_t Mesh::findMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if (allowedTypes & (1 << i) &&
			(memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type index for vertex memory allocation!");
	return -1;
}
