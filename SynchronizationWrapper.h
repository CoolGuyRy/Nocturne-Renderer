#ifndef SYNCHRONIZATION_WRAPPER_H
#define SYNCHRONIZATION_WRAPPER_H

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;

/*

	

*/

class SemaphoreWrapper {
public:
	SemaphoreWrapper(LogicalDeviceWrapper*);
	~SemaphoreWrapper();

	VkSemaphore GetSemaphore();
private:
	void CreateSemaphore();

	VkSemaphore mSemaphore;

	LogicalDeviceWrapper* mLogicalDevice;
};

class FenceWrapper {
public:
	FenceWrapper(LogicalDeviceWrapper*, VkFenceCreateFlags);
	~FenceWrapper();

	VkFence GetFence();
private:
	void CreateFence(VkFenceCreateFlags);

	VkFence mFence;

	LogicalDeviceWrapper* mLogicalDevice;
};

#endif
