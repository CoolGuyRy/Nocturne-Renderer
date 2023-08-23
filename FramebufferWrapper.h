#ifndef FRAME_BUFFER_WRAPPER_H
#define FRAME_BUFFER_WRAPPER_H

#include <vector>
#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class SwapchainWrapper;
class RenderPassWrapper;
class ImageViewWrapper;

/*

	

*/

class FramebufferWrapper {
public:
	FramebufferWrapper(LogicalDeviceWrapper*, SwapchainWrapper*, RenderPassWrapper*, int);
	FramebufferWrapper(LogicalDeviceWrapper*, SwapchainWrapper*, RenderPassWrapper*, int, ImageViewWrapper*);
	~FramebufferWrapper();

	VkFramebuffer GetFramebuffer();
private:
	void CreateFramebuffer(int);
	void CreateFramebuffer(int, ImageViewWrapper*);

	VkFramebuffer mFramebuffer;

	LogicalDeviceWrapper* mLogicalDevice;
	SwapchainWrapper* mSwapchain;
	RenderPassWrapper* mRenderPass;
};

#endif