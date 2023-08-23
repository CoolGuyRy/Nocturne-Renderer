#ifndef SAMPLER_WRAPPER_H
#define SAMPLER_WRAPPER_H

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;

class SamplerWrapper {
public:
	SamplerWrapper(LogicalDeviceWrapper*);
	~SamplerWrapper();

	VkSampler GetSampler();
private:
	void CreateSampler();

	VkSampler mSampler;

	LogicalDeviceWrapper* mLogicalDevice;
};

#endif