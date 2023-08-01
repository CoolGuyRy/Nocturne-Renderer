#ifndef PHYSICAL_DEVICE_WRAPPER_H
#define PHYSICAL_DEVICE_WRAPPER_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

/*

	TODO: Find better criteria for suitable devices.

*/

class PhysicalDeviceWrapper {
public:
	PhysicalDeviceWrapper(VkInstance);
	~PhysicalDeviceWrapper();

	VkPhysicalDevice GetPhysicalDevice();
	VkPhysicalDeviceProperties GetPhysicalDeviceProperties();
	VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures();
	VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties();
private:
	void RetrievePhysicalDevice();

	bool CheckDeviceSuitable(VkPhysicalDevice);

	VkInstance mInstance;
	VkPhysicalDevice mPhysicalDevice;
	VkPhysicalDeviceProperties mPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
	VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
};
#endif