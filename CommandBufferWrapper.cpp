#include "CommandBufferWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "CommandPoolWrapper.h"

CommandBufferWrapper::CommandBufferWrapper(LogicalDeviceWrapper* lDevice, CommandPoolWrapper* commandpool) : mLogicalDevice(lDevice), mCommandPool(commandpool) {
	AllocateCommandBuffer();
}

CommandBufferWrapper::~CommandBufferWrapper() {
	vkFreeCommandBuffers(mLogicalDevice->GetLogicalDevice(), mCommandPool->GetCommandPool(), 1, &mCommandBuffer); std::cout << "Success: Command Buffer freed." << std::endl;
}

VkCommandBuffer CommandBufferWrapper::GetCommandBuffer() {
	return mCommandBuffer;
}

void CommandBufferWrapper::AllocateCommandBuffer() {
	VkCommandBufferAllocateInfo commandBufferAI = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,			// sType
		nullptr,												// pNext
		mCommandPool->GetCommandPool(),							// commandPool
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,						// level
		1														// commandBufferCount
	};

	VkResult result = vkAllocateCommandBuffers(mLogicalDevice->GetLogicalDevice(), &commandBufferAI, &mCommandBuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Command Buffer allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate command buffer!");
	}
}
