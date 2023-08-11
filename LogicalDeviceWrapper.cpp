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
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType
		nullptr,															// pNext
		0,																	// flags
		(uint32_t)mPhysicalDevice->GetQueueFamilyIndices().mGraphics,		// queueFamilyIndex
		1,																	// queueCount
		&queuePriority														// pQueuePriorities
	};
	VkDeviceQueueCreateInfo presentQueueCI = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType
		nullptr,															// pNext
		0,																	// flags
		(uint32_t)mPhysicalDevice->GetQueueFamilyIndices().mPresent,		// queueFamilyIndex
		1,																	// queueCount
		&queuePriority														// pQueuePriorities
	};
	VkDeviceQueueCreateInfo transferQueueCI = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,							// sType
		nullptr,															// pNext
		0,																	// flags
		(uint32_t)mPhysicalDevice->GetQueueFamilyIndices().mTransfer,		// queueFamilyIndex
		1,																	// queueCount
		&queuePriority														// pQueuePriorities
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
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,								// sType
		nullptr,															// pNext
		0,																	// flags
		(uint32_t)queueCIs.size(),											// queueCreateInfoCount
		queueCIs.data(),													// pQueueCreateInfos
		0,																	// enabledLayerCount (Deprecated)
		nullptr,															// ppEnabledLayerNames (Deprecated)
		(uint32_t)ENABLED_LOGICAL_DEVICE_EXTENSIONS.size(),					// enabledExtensionCount
		ENABLED_LOGICAL_DEVICE_EXTENSIONS.data(),							// ppEnabledExtensionNames
		&ENABLED_PHYSICAL_DEVICE_FEATURES									// pEnabledFeatures
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
