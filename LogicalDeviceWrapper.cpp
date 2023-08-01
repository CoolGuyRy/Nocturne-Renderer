#include "LogicalDeviceWrapper.h"

LogicalDeviceWrapper::LogicalDeviceWrapper(PhysicalDeviceWrapper* pDevice) : mPhysicalDevice(pDevice) {
	CreateLogicalDevice();
}

LogicalDeviceWrapper::~LogicalDeviceWrapper() {
	vkDestroyDevice(mLogicalDevice, nullptr); std::cout << "Success: Logical Device Destroyed." << std::endl;
}

VkDevice LogicalDeviceWrapper::GetLogicalDevice() {
	return mLogicalDevice;
}

VkQueue LogicalDeviceWrapper::GetGraphicsQueue() {
	return mGraphicsQueue;
}

void LogicalDeviceWrapper::CreateLogicalDevice() {
	// Describe the queues to be created on the logical device
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCI = { 
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,				// sType
		nullptr,												// pNext
		0,														// flags
		mPhysicalDevice->GetQueueFamilyIndices().mGraphics,		// queueFamilyIndex
		1,														// queueCount
		&queuePriority											// pQueuePriorities
	};

	if (!CheckDeviceExtensionSupport()) {
		throw std::runtime_error("Failed to create a Logical Device that supports all required extensions!");
	}

	VkDeviceCreateInfo deviceCI = { 
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,					// sType
		nullptr,												// pNext
		0,														// flags
		1,														// queueCreateInfoCount
		&queueCI,												// pQueueCreateInfos
		0,														// enabledLayerCount (Deprecated)
		nullptr,												// ppEnabledLayerNames (Deprecated)
		(uint32_t)ENABLED_LOGICAL_DEVICE_EXTENSIONS.size(),		// enabledExtensionCount
		ENABLED_LOGICAL_DEVICE_EXTENSIONS.data(),				// ppEnabledExtensionNames
		&ENABLED_PHYSICAL_DEVICE_FEATURES						// pEnabledFeatures
	};

	VkResult result = vkCreateDevice(mPhysicalDevice->GetPhysicalDevice(), &deviceCI, nullptr, &mLogicalDevice);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Logical Device Created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a Logical Device! Error Code: " + NT_CHECK_RESULT(result)); // not tested
	}
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
			if (strcmp(ENABLED_VALIDATION_LAYERS.at(i), availableExtensions.at(j).extensionName) == 0) {
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