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

	VkSwapchainKHR GetSwapchain();
	VkExtent2D GetSwapchainExtent();
	std::vector<SwapchainImage> GetSwapchainImages();
private:
	void CreateSwapchain();

	VkSwapchainKHR mSwapchain;
	std::vector<SwapchainImage> mSwapchainImages;
	VkExtent2D mSwapchainExtent;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SurfaceWrapper* mSurface;
};
#endif
