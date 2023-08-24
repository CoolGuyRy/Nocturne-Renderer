#ifndef SAMPLER_WRAPPER_H
#define SAMPLER_WRAPPER_H

#include <vulkan/vulkan.h>

class Context;
class LogicalDeviceWrapper;

class SamplerWrapper {
public:
	SamplerWrapper(Context*);
	~SamplerWrapper();

	VkSampler GetSampler();
private:
	void CreateSampler();

	VkSampler mSampler;

	Context* mContext;
};

#endif