#ifndef SWAPCHAIN_WRAPPER_H
#define SWAPCHAIN_WRAPPER_H

#include <vulkan/vulkan.h>

#include <vector>

class SurfaceWrapper;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;

/*

	

*/

class SwapchainWrapper {
public:
	SwapchainWrapper(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, SurfaceWrapper*);
	~SwapchainWrapper();
private:
	void CreateSwapchain();

	VkSwapchainKHR mSwapchain;
	std::vector<VkSurfaceCapabilitiesKHR> mSurfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> mSurfaceFormats;
	std::vector<VkPresentModeKHR> mPresentModes;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SurfaceWrapper* mSurface;
};

#endif