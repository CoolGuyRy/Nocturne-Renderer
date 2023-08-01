#ifndef PHYSICAL_DEVICE_WRAPPER_H
#define PHYSICAL_DEVICE_WRAPPER_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

#include "globals.h"

#include "InstanceWrapper.h"

/*

	TODO: Find better criteria for suitable devices.
	TODO: Add Surface Support Queue Family Index Check

*/

struct QueueFamilyIndices {
	int mGraphics = -1;
	int mPresent = -1;					// Need to implement
	int mCompute = -1;
	int mTransfer = -1;
	int mSparseBinding = -1;
};

class PhysicalDeviceWrapper {
public:
	PhysicalDeviceWrapper(InstanceWrapper*);
	~PhysicalDeviceWrapper();

	VkPhysicalDevice GetPhysicalDevice();

	VkPhysicalDeviceProperties GetPhysicalDeviceProperties();
	VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures();
	VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties();

	QueueFamilyIndices GetQueueFamilyIndices();
private:
	void RetrievePhysicalDevice();

	bool CheckDeviceSuitable(VkPhysicalDevice);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice);
	
	void AssignQueueFamilyIndices();

	InstanceWrapper* mInstance;
	VkPhysicalDevice mPhysicalDevice;
	VkPhysicalDeviceProperties mPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
	VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
	QueueFamilyIndices mQueueFamilyIndices;
};
#endif