#include "Buffer.h"
#include "globals.h"

Buffer::Buffer(VkPhysicalDevice pDevice, VkDevice lDevice, VkDeviceSize dSize, VkBufferUsageFlags uFlags, VkMemoryPropertyFlags pFlags) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice) {
	CreateBuffer(dSize, uFlags, pFlags);
}
Buffer::~Buffer() {
	vkDestroyBuffer(mLogicalDevice, mBuffer, nullptr); std::cout << "Success: Buffer destroyed." << std::endl;
	vkFreeMemory(mLogicalDevice, mBufferMemory, nullptr); std::cout << "Success: Buffer Memory freed." << std::endl;
}
void Buffer::MapBufferMemory(void* iData, VkDeviceSize dSize) {
	void* data;
	vkMapMemory(mLogicalDevice, mBufferMemory, 0, dSize, 0, &data);
	memcpy(data, iData, static_cast<size_t>(dSize));
	vkUnmapMemory(mLogicalDevice, mBufferMemory);
}
VkBuffer Buffer::GetBuffer() {
	return mBuffer;
}
VkDeviceMemory Buffer::GetBufferMemory() {
	return mBufferMemory;
}
void Buffer::CreateBuffer(VkDeviceSize dSize, VkBufferUsageFlags uFlags, VkMemoryPropertyFlags pFlags) {
	// Create Buffer struct
	VkBufferCreateInfo bufferCI = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.size = dSize,
		.usage = uFlags,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr
	};

	// Create Buffer
	VkResult result = vkCreateBuffer(mLogicalDevice, &bufferCI, nullptr, &mBuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Buffer created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Grab Buffer Memory Requirements
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(mLogicalDevice, mBuffer, &memoryRequirements);

	// Allocate Memory for Buffer struct
	VkMemoryAllocateInfo memoryAI = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext = nullptr,
		.allocationSize = memoryRequirements.size,
		.memoryTypeIndex = FindMemoryTypeIndex(mPhysicalDevice, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};

	// Allocate Memory for Buffer
	result = vkAllocateMemory(mLogicalDevice, &memoryAI, nullptr, &mBufferMemory);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Buffer Memory allocated!" << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Buffer Memory! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Bind Buffer Memory
	vkBindBufferMemory(mLogicalDevice, mBuffer, mBufferMemory, 0);
}

void CopyBuffer(VkDevice mLogicalDevice, VkCommandPool pool, VkQueue transferQueue, Buffer* srcBuffer, Buffer* dstBuffer, VkDeviceSize bufferSize) {
	// Create Transfer Command Buffer
	VkCommandBuffer transferCommandBuffer;
	VkCommandBufferAllocateInfo commandBufferAI = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = nullptr,
		.commandPool = pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};

	VkResult result = vkAllocateCommandBuffers(mLogicalDevice, &commandBufferAI, &transferCommandBuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Command Buffer allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Begin Recording Transfer Command Buffer struct
	VkCommandBufferBeginInfo commandBufferBI = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,																						// sType
		nullptr,																															// pNext
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,																						// flags
		nullptr																																// pInheritanceInfo
	};

	// Begin Recording Transfer Command Buffer
	result = vkBeginCommandBuffer(transferCommandBuffer, &commandBufferBI);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkBufferCopy bufferCopy = {
		0,																																// srcOffset
		0,																																// dstOffset
		bufferSize																														// size
	};

	vkCmdCopyBuffer(transferCommandBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &bufferCopy);

	result = vkEndCommandBuffer(transferCommandBuffer);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to end recording transfer command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Submit Transfer Command Buffer struct
	VkCommandBuffer buffer = transferCommandBuffer;
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
	result = vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit copy command to queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Wait for Transfer Queue to finish
	result = vkQueueWaitIdle(transferQueue);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Data transferred." << std::endl;
	} else {
		throw std::runtime_error("Failed to wait for transfer queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Free Transfer Command Buffer
	vkFreeCommandBuffers(mLogicalDevice, pool, 1, &buffer);
}