#ifndef RENDER_PASS_WRAPPER_H
#define RENDER_PASS_WRAPPER_H

#include <vulkan/vulkan.h>

class SurfaceWrapper;
class LogicalDeviceWrapper;

/*

	

*/

class RenderPassWrapper {
public:
	RenderPassWrapper(LogicalDeviceWrapper*, SurfaceWrapper*);
	~RenderPassWrapper();

	VkRenderPass GetRenderPass();
private:
	void CreateGenericRenderPass();
	void CreateDepthRenderPass();

	VkRenderPass mRenderPass;

	LogicalDeviceWrapper* mLogicalDevice;
	SurfaceWrapper* mSurface;

};

#endif