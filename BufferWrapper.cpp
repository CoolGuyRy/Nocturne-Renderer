#include "BufferWrapper.h"
#include "globals.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "CommandPoolWrapper.h"
#include "CommandBufferWrapper.h"

BufferWrapper::BufferWrapper(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, VkDeviceSize dSize, VkBufferUsageFlags uFlags, VkMemoryPropertyFlags pFlags) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice) {
	CreateBuffer(dSize, uFlags, pFlags);
}

BufferWrapper::~BufferWrapper() {
	vkDestroyBuffer(mLogicalDevice->GetLogicalDevice(), mBuffer, nullptr); std::cout << "Success: Vertex Buffer destroyed." << std::endl;
	vkFreeMemory(mLogicalDevice->GetLogicalDevice(), mBufferMemory, nullptr); std::cout << "Success: Vertex Buffer Memory freed." << std::endl;
}

void BufferWrapper::MapBufferMemory(void* iData, VkDeviceSize dSize) {
	void* data;
	vkMapMemory(mLogicalDevice->GetLogicalDevice(), mBufferMemory, 0, dSize, 0, &data);
	memcpy(data, iData, (size_t)dSize);
	vkUnmapMemory(mLogicalDevice->GetLogicalDevice(), mBufferMemory);
}

VkBuffer BufferWrapper::GetBuffer() {
	return mBuffer;
}

VkDeviceMemory BufferWrapper::GetBufferMemory() {
	return mBufferMemory;
}

void BufferWrapper::CreateBuffer(VkDeviceSize dSize, VkBufferUsageFlags uFlags, VkMemoryPropertyFlags pFlags) {
	// Create Vertex Buffer struct
	VkBufferCreateInfo bufferCI = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,																								// sType
		nullptr,																															// pNext
		0,																																	// flags
		dSize,																																// size
		uFlags,																																// usage
		VK_SHARING_MODE_EXCLUSIVE,																											// sharingMode
		0,																																	// queueFamilyIndexCount
		nullptr																																// pQueueFamilyIndices
	};

	// Create Vertex Buffer
	VkResult result = vkCreateBuffer(mLogicalDevice->GetLogicalDevice(), &bufferCI, nullptr, &mBuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Vertex Buffer created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Vertex Buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Grab Buffer Memory Requirements
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mLogicalDevice->GetLogicalDevice(), mBuffer, &memoryRequirements);

	// Allocate Memory for Vertex Buffer struct
	VkMemoryAllocateInfo memoryAI = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,																								// sType
		nullptr,																															// pNext
		memoryRequirements.size,																											// allocationSize
		FindMemoryTypeIndex(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)	// memoryTypeIndex
	};

	// Allocate Memory for Vertex Buffer
	result = vkAllocateMemory(mLogicalDevice->GetLogicalDevice(), &memoryAI, nullptr, &mBufferMemory);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Vertex Buffer Memory allocated!" << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Vertex Buffer Memory! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Bind Vertex Buffer Memory
	vkBindBufferMemory(mLogicalDevice->GetLogicalDevice(), mBuffer, mBufferMemory, 0);
}

/*

	This function is used to find the memory type index that has all the required property bits set.
	It loops through all the memory types available on the device and checks if their bit field matches
	the desired properties. Once it finds a memory type that fits all the properties we need, it returns

*/
uint32_t BufferWrapper::FindMemoryTypeIndex(uint32_t allowedTypes, VkMemoryPropertyFlags properties) {
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

void CopyBuffer(LogicalDeviceWrapper* lDevice, CommandPoolWrapper* tCommandPool, BufferWrapper* srcBuffer, BufferWrapper* dstBuffer, VkDeviceSize bufferSize) {
	// Create Transfer Command Buffer
	CommandBufferWrapper* transferCommandBuffer = new CommandBufferWrapper(lDevice, tCommandPool);

	// Begin Recording Transfer Command Buffer struct
	VkCommandBufferBeginInfo commandBufferBI = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,																						// sType
		nullptr,																															// pNext
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,																						// flags
		nullptr																																// pInheritanceInfo
	};

	// Begin Recording Transfer Command Buffer
	VkResult result = vkBeginCommandBuffer(transferCommandBuffer->GetCommandBuffer(), &commandBufferBI);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}
	
		VkBufferCopy bufferCopy = {
			0,																																// srcOffset
			0,																																// dstOffset
			bufferSize																														// size
		};

		vkCmdCopyBuffer(transferCommandBuffer->GetCommandBuffer(), srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &bufferCopy);

	result = vkEndCommandBuffer(transferCommandBuffer->GetCommandBuffer());
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to end recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Submit Transfer Command Buffer struct
	VkCommandBuffer buffer = transferCommandBuffer->GetCommandBuffer();
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,																										// sType
		nullptr,																															// pNext
		0,																																	// waitSemaphoreCount
		nullptr,																															// pWaitSemaphores
		nullptr,																															// pWaitDstStageMask
		1,																																	// commandBufferCount
		&buffer,																															// pCommandBuffers
		0,																																	// signalSemaphoreCount
		nullptr																																// pSignalSemaphores
	};

	// Submit Transfer Command Buffer
	result = vkQueueSubmit(lDevice->GetTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit copy command to queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Wait for Transfer Queue to finish
	result = vkQueueWaitIdle(lDevice->GetTransferQueue());
	if (result == VK_SUCCESS) {
		std::cout << "Success: Data transferred." << std::endl;
	} else {
		throw std::runtime_error("Failed to wait for transfer queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Free Transfer Command Buffer
	vkFreeCommandBuffers(lDevice->GetLogicalDevice(), tCommandPool->GetCommandPool(), 1, &buffer);
}