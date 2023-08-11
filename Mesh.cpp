#include "Mesh.h"
#include "globals.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"

Mesh::Mesh(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, std::vector<Vertex>* vertices) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice) {
	CreateVertexBuffer(vertices);
}

Mesh::~Mesh() {
	vkDestroyBuffer(mLogicalDevice->GetLogicalDevice(), mVertexBuffer, nullptr); std::cout << "Success: Vertex Buffer destroyed." << std::endl;
	vkFreeMemory(mLogicalDevice->GetLogicalDevice(), mVertexBufferMemory, nullptr); std::cout << "Success: Vertex Buffer Memory freed." << std::endl;
}

int Mesh::GetVertexCount() {
	return mVertexCount;
}

VkBuffer Mesh::GetVertexBuffer() {
	return mVertexBuffer;
}

void Mesh::CreateVertexBuffer(std::vector<Vertex>* vertices) {
	// Set Vertex Count Information
	mVertexCount = (int)vertices->size();

	// Create Vertex Buffer struct
	VkBufferCreateInfo bufferCI = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,																								// sType
		nullptr,																															// pNext
		0,																																	// flags
		sizeof(Vertex) * mVertexCount,																										// size
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,																									// usage
		VK_SHARING_MODE_EXCLUSIVE,																											// sharingMode
		0,																																	// queueFamilyIndexCount
		nullptr																																// pQueueFamilyIndices
	};

	// Create Vertex Buffer
	VkResult result = vkCreateBuffer(mLogicalDevice->GetLogicalDevice(), &bufferCI, nullptr, &mVertexBuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Vertex Buffer created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Vertex Buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Grab Buffer Memory Requirements
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mLogicalDevice->GetLogicalDevice(), mVertexBuffer, &memoryRequirements);

	// Allocate Memory for Vertex Buffer struct
	VkMemoryAllocateInfo memoryAI = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,																								// sType
		nullptr,																															// pNext
		memoryRequirements.size,																											// allocationSize
		FindMemoryTypeIndex(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)	// memoryTypeIndex
	};

	// Allocate Memory for Vertex Buffer
	result = vkAllocateMemory(mLogicalDevice->GetLogicalDevice(), &memoryAI, nullptr, &mVertexBufferMemory);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Vertex Buffer Memory allocated!" << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Vertex Buffer Memory! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Bind Vertex Buffer Memory
	vkBindBufferMemory(mLogicalDevice->GetLogicalDevice(), mVertexBuffer, mVertexBufferMemory, 0);

	// Map Vertex Buffer Memory
	void* data;

	vkMapMemory(mLogicalDevice->GetLogicalDevice(), mVertexBufferMemory, 0, bufferCI.size, 0, &data);

	memcpy(data, vertices->data(), (size_t)bufferCI.size);

	vkUnmapMemory(mLogicalDevice->GetLogicalDevice(), mVertexBufferMemory);
}


/*

	This function is used to find the memory type index that has all the required property bits set.
	It loops through all the memory types available on the device and checks if their bit field matches
	the desired properties. Once it finds a memory type that fits all the properties we need, it returns

*/
uint32_t Mesh::FindMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice->GetPhysicalDevice(), &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if ((allowedTypes & (1 << i)) && ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)) {
			return i; // Valid Memory type, so return its index
		}
	}

	throw std::runtime_error("Failed to find a suitable memory type!");
	
	return UINT32_MAX;
}