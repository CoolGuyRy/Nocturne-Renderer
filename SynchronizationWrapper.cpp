#include "SynchronizationWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"

SemaphoreWrapper::SemaphoreWrapper(LogicalDeviceWrapper* lDevice) : mLogicalDevice(lDevice) {
	CreateSemaphore();
}

SemaphoreWrapper::~SemaphoreWrapper() {
	vkDestroySemaphore(mLogicalDevice->GetLogicalDevice(), mSemaphore, nullptr); std::cout << "Success: Semaphore destroyed." << std::endl;
}

VkSemaphore SemaphoreWrapper::GetSemaphore() {
	return mSemaphore;
}

void SemaphoreWrapper::CreateSemaphore() {
	VkSemaphoreCreateInfo semaphoreCI = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,			// sType
		nullptr,											// pNext
		0													// flags
	};

	VkResult result = vkCreateSemaphore(mLogicalDevice->GetLogicalDevice(), &semaphoreCI, nullptr, &mSemaphore);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Semaphore created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a semaphore! Error Code: " + NT_CHECK_RESULT(result));
	}
}

FenceWrapper::FenceWrapper(LogicalDeviceWrapper* lDevice, VkFenceCreateFlags flags) : mLogicalDevice(lDevice) {
	CreateFence(flags);
}

FenceWrapper::~FenceWrapper() {
	vkDestroyFence(mLogicalDevice->GetLogicalDevice(), mFence, nullptr); std::cout << "Success: Fence destroyed." << std::endl;
}

VkFence FenceWrapper::GetFence() {
	return mFence;
}

void FenceWrapper::CreateFence(VkFenceCreateFlags flags) {
	VkFenceCreateInfo fenceCI = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,				// sType
		nullptr,											// pNext
		flags												// flags
	};

	VkResult result = vkCreateFence(mLogicalDevice->GetLogicalDevice(), &fenceCI, nullptr, &mFence);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Fence created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create fence! Error Code: " + NT_CHECK_RESULT(result));
	}
}
