#ifndef SWAPCHAIN_WRAPPER_H
#define SWAPCHAIN_WRAPPER_H

#include <vulkan/vulkan.h>

#include <vector>

class SurfaceWrapper;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class ImageViewWrapper;

/*

	

*/

struct SwapchainImage {
	VkImage mImage;
	ImageViewWrapper* mImageView;
};

class SwapchainWrapper {
public:
	SwapchainWrapper(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, SurfaceWrapper*);
	~SwapchainWrapper();
private:
	void CreateSwapchain();

	VkSwapchainKHR mSwapchain;
	std::vector<SwapchainImage> mSwapchainImages;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SurfaceWrapper* mSurface;
};

#endif