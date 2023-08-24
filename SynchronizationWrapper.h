#ifndef SYNCHRONIZATION_WRAPPER_H
#define SYNCHRONIZATION_WRAPPER_H

#include <vulkan/vulkan.h>

class Context;
class LogicalDeviceWrapper;

/*

	

*/

class SemaphoreWrapper {
public:
	SemaphoreWrapper(Context*);
	~SemaphoreWrapper();

	VkSemaphore GetSemaphore();
private:
	void CreateSemaphore();

	VkSemaphore mSemaphore;

	Context* mContext;
};

class FenceWrapper {
public:
	FenceWrapper(Context*, VkFenceCreateFlags);
	~FenceWrapper();

	VkFence GetFence();
private:
	void CreateFence(VkFenceCreateFlags);

	VkFence mFence;

	Context* mContext;
};

#endif
