#ifndef LOGICAL_DEVICE_WRAPPER_H
#define LOGICAL_DEVICE_WRAPPER_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class PhysicalDeviceWrapper;

/*

	Notes:
		- I made a design decision to keep the Queues together with the Logical Device. 
		  We shall see how this goes. My thought process behind this was that since
		  both initialization and cleanup of the queues are done in the same place, it
		  would make sense to keep them together. I also think that it would be easier 
		  to keep track of.

*/

class LogicalDeviceWrapper {
public:
	LogicalDeviceWrapper(PhysicalDeviceWrapper*);
	~LogicalDeviceWrapper();

	VkDevice GetLogicalDevice();
	VkQueue GetGraphicsQueue();
	VkQueue GetPresentQueue();
private:
	void CreateLogicalDevice();

	bool CheckDeviceExtensionSupport();

	VkDevice mLogicalDevice;
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;

	PhysicalDeviceWrapper* mPhysicalDevice;
};
#endif