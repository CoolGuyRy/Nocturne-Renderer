#ifndef PHYSICAL_DEVICE_WRAPPER_H
#define PHYSICAL_DEVICE_WRAPPER_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class InstanceWrapper;
class SurfaceWrapper;

/*

	TODO: Find better criteria for suitable devices.
	TODO: Add Surface Support Queue Family Index Check

*/

struct QueueFamilyIndices {
	int mGraphics = -1;
	int mPresent = -1;
	int mCompute = -1;
	int mTransfer = -1;
	int mSparseBinding = -1;
};

class PhysicalDeviceWrapper {
public:
	PhysicalDeviceWrapper(InstanceWrapper*, SurfaceWrapper*);
	~PhysicalDeviceWrapper();

	VkPhysicalDevice GetPhysicalDevice();

	VkPhysicalDeviceProperties GetPhysicalDeviceProperties();
	VkPhysicalDeviceFeatures GetPhysicalDeviceFeatures();
	VkPhysicalDeviceMemoryProperties GetPhysicalDeviceMemoryProperties();

	QueueFamilyIndices& GetQueueFamilyIndices();
private:
	void RetrievePhysicalDevice();

	void OutputPhysicalDeviceExtensions();

	bool CheckDeviceSuitable(VkPhysicalDevice);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice);
	
	void AssignQueueFamilyIndices();
	void ValidateQueueFamilyIndices();

	VkPhysicalDevice mPhysicalDevice;
	VkPhysicalDeviceProperties mPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
	VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
	QueueFamilyIndices mQueueFamilyIndices;

	InstanceWrapper* mInstance;
	SurfaceWrapper* mSurface;
};
#endif
