#include "PhysicalDeviceWrapper.h"

PhysicalDeviceWrapper::PhysicalDeviceWrapper(VkInstance instance) : mInstance(instance) {
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

void PhysicalDeviceWrapper::RetrievePhysicalDevice() {
	// Get the number of physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	// If there are no devices with Vulkan support, throw an error
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	// Create a list of physical devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	for (size_t i = 0; i < devices.size(); i++) {
		if (CheckDeviceSuitable(devices.at(i))) {
			mPhysicalDevice = devices.at(i);
			break;
		}
	}

	// Check if a suitable device was found
	if (mPhysicalDevice != nullptr) {
		std::cout << "Success: Found a suitable Physical Device." << std::endl;
	} else {
		throw std::runtime_error("Failed to find a suitable Physical Device!");
	}
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
