#include "CommandPoolWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"

CommandPoolWrapper::CommandPoolWrapper(LogicalDeviceWrapper* lDevice, uint32_t index) : mLogicalDevice(lDevice) {
	CreateCommandPool(index);
}

CommandPoolWrapper::~CommandPoolWrapper() {
	vkDestroyCommandPool(mLogicalDevice->GetLogicalDevice(), mCommandPool, nullptr); std::cout << "Success: Command pool destroyed" << std::endl;
}

VkCommandPool CommandPoolWrapper::GetCommandPool() {
	return mCommandPool;
}

void CommandPoolWrapper::CreateCommandPool(uint32_t index) {
	VkCommandPoolCreateInfo poolCI = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			// sType
		nullptr,											// pNext
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	// flags
		index												// queueFamilyIndex
	};

	VkResult result = vkCreateCommandPool(mLogicalDevice->GetLogicalDevice(), &poolCI, nullptr, &mCommandPool);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Command pool created" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Command Pool! Error Code: " + NT_CHECK_RESULT(result));
	}
}
