#include "PhysicalDeviceWrapper.h"

PhysicalDeviceWrapper::PhysicalDeviceWrapper(InstanceWrapper* instance) : mInstance(instance) {
	RetrievePhysicalDevice();
}

PhysicalDeviceWrapper::~PhysicalDeviceWrapper() {
}

VkPhysicalDevice PhysicalDeviceWrapper::GetPhysicalDevice() {
	return mPhysicalDevice;
}

VkPhysicalDeviceProperties PhysicalDeviceWrapper::GetPhysicalDeviceProperties() {
	return mPhysicalDeviceProperties;
}

VkPhysicalDeviceFeatures PhysicalDeviceWrapper::GetPhysicalDeviceFeatures() {
	return mPhysicalDeviceFeatures;
}

VkPhysicalDeviceMemoryProperties PhysicalDeviceWrapper::GetPhysicalDeviceMemoryProperties() {
	return mPhysicalDeviceMemoryProperties;
}

QueueFamilyIndices PhysicalDeviceWrapper::GetQueueFamilyIndices() {
	return mQueueFamilyIndices;
}

void PhysicalDeviceWrapper::RetrievePhysicalDevice() {
	// Get the number of physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance->GetInstance(), &deviceCount, nullptr);

	// If there are no devices with Vulkan support, throw an error
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	// Create a list of physical devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance->GetInstance(), &deviceCount, devices.data());

	for (size_t i = 0; i < devices.size(); i++) {
		if (!CheckDeviceSuitable(devices.at(i))) {
			continue;
		}
		if (!CheckDeviceExtensionSupport(devices.at(i))) {
			continue;
		}
		mPhysicalDevice = devices.at(i);
		break;
	}

	// Check if a suitable device was found
	if (mPhysicalDevice != nullptr) {
		std::cout << "Success: Found a suitable Physical Device." << std::endl;
	} else {
		throw std::runtime_error("Failed to find a suitable Physical Device!");
	}

	AssignQueueFamilyIndices();
}

/*

	This function checks to see if the physical device is suitable for the application.
	This function also assigns the properties, features, and memory properties struct of the physical device.
	Criteria for Suitable Devices: (idk what to look, for so we roll)
		- Discrete GPU
		- Integrated GPU
		- Virtual GPU
		- CPU

*/
bool PhysicalDeviceWrapper::CheckDeviceSuitable(VkPhysicalDevice pDevice) {
	vkGetPhysicalDeviceProperties(pDevice, &mPhysicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(pDevice, &mPhysicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(pDevice, &mPhysicalDeviceMemoryProperties);

	switch (mPhysicalDeviceProperties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			return true;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			return true;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			return true;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return true;
			break;
		default:
			return false;
			mPhysicalDeviceProperties = { };
			mPhysicalDeviceFeatures = { };
			mPhysicalDeviceMemoryProperties = { };
			break;
	}
}

/*
	This function checks to see if the physical device has the required extensions.
*/
bool PhysicalDeviceWrapper::CheckDeviceExtensionSupport(VkPhysicalDevice pDevice) {
	// Get the number of extensions
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);

	// Create a list of extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());

	// Check if the required extensions are in the list of available extensions
	for (size_t i = 0; i < ENABLED_PHYSICAL_DEVICE_EXTENSIONS.size(); i++) {
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

/*

	This function assigns the queue family indices of the physical device.

*/
void PhysicalDeviceWrapper::AssignQueueFamilyIndices() {
	// Get the number of queue families
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);

	// Create a list of queue families
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	// Iterate through Queue Family Properties and assign the indices
	for (uint32_t i = 0; i < queueFamilies.size(); i++) {
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			mQueueFamilyIndices.mGraphics = i;
		}
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_COMPUTE_BIT) {
			mQueueFamilyIndices.mCompute = i;
		}
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_TRANSFER_BIT) {
			mQueueFamilyIndices.mTransfer = i;
		}
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
			mQueueFamilyIndices.mSparseBinding = i;
		}
	}
}
