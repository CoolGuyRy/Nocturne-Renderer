#include "LogicalDeviceWrapper.h"
#include "globals.h"
#include "PhysicalDeviceWrapper.h"

LogicalDeviceWrapper::LogicalDeviceWrapper(PhysicalDeviceWrapper* pDevice) : mPhysicalDevice(pDevice) {
	CreateLogicalDevice();
}

LogicalDeviceWrapper::~LogicalDeviceWrapper() {
	vkDestroyDevice(mLogicalDevice, nullptr); std::cout << "Success: Logical Device destroyed." << std::endl;
}

VkDevice LogicalDeviceWrapper::GetLogicalDevice() {
	return mLogicalDevice;
}

VkQueue LogicalDeviceWrapper::GetGraphicsQueue() {
	return mGraphicsQueue;
}

VkQueue LogicalDeviceWrapper::GetPresentQueue() {
	return mPresentQueue;
}

VkQueue LogicalDeviceWrapper::GetTransferQueue() {
	return mTransferQueue;
}

void LogicalDeviceWrapper::CreateLogicalDevice() {
	// Describe the queues to be created on the logical device
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueCI = { 
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)mPhysicalDevice->GetQueueFamilyIndices().mGraphics,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};
	VkDeviceQueueCreateInfo presentQueueCI = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)mPhysicalDevice->GetQueueFamilyIndices().mPresent,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};
	VkDeviceQueueCreateInfo transferQueueCI = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)mPhysicalDevice->GetQueueFamilyIndices().mTransfer,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};

	// Create an array to pass to deviceCI
	std::vector<VkDeviceQueueCreateInfo> queueCIs = { graphicsQueueCI, transferQueueCI };
	
	// Check if the present queue is the same as the graphics queue
	if (mPhysicalDevice->GetQueueFamilyIndices().mGraphics != mPhysicalDevice->GetQueueFamilyIndices().mPresent) {
		queueCIs.push_back(presentQueueCI);
	}

	// Check if the logical device supports all required extensions
	if (!CheckDeviceExtensionSupport()) {
		throw std::runtime_error("Failed to create a Logical Device that supports all required extensions!");
	}

	// Describe the logical device to be created
	VkDeviceCreateInfo deviceCI = { 
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueCreateInfoCount = (uint32_t)queueCIs.size(),
		.pQueueCreateInfos = queueCIs.data(),
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = (uint32_t)ENABLED_LOGICAL_DEVICE_EXTENSIONS.size(),
		.ppEnabledExtensionNames = ENABLED_LOGICAL_DEVICE_EXTENSIONS.data(),
		.pEnabledFeatures = &ENABLED_PHYSICAL_DEVICE_FEATURES
	};

	// Create the logical device
	VkResult result = vkCreateDevice(mPhysicalDevice->GetPhysicalDevice(), &deviceCI, nullptr, &mLogicalDevice);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Logical Device created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a Logical Device! Error Code: " + NT_CHECK_RESULT(result)); // not tested
	}

	// Get the created queue handles
	vkGetDeviceQueue(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mGraphics, 0, &mGraphicsQueue);
	vkGetDeviceQueue(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mPresent, 0, &mPresentQueue);
	vkGetDeviceQueue(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mTransfer, 0, &mTransferQueue);
}

/*

	Checks if the Logical Device supports all required extensions

*/
bool LogicalDeviceWrapper::CheckDeviceExtensionSupport() {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(mPhysicalDevice->GetPhysicalDevice(), nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(mPhysicalDevice->GetPhysicalDevice(), nullptr, &extensionCount, availableExtensions.data());

	for (size_t i = 0; i < ENABLED_LOGICAL_DEVICE_EXTENSIONS.size(); i++) {
		bool found = false;
		for (uint32_t j = 0; j < availableExtensions.size(); j++) {
			if (strcmp(ENABLED_LOGICAL_DEVICE_EXTENSIONS.at(i), availableExtensions.at(j).extensionName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}

	return true;
}
