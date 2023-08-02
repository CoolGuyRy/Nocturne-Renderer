#ifndef SURFACE_WRAPPER_H
#define SURFACE_WRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class WindowWrapper;
class InstanceWrapper;

/*

	TODO: Establish better criteria for choosing the best surface format and present mode.

*/

class SurfaceWrapper {
public:
	SurfaceWrapper(WindowWrapper*, InstanceWrapper*);
	~SurfaceWrapper();

	void AcquireSurfaceProperties(VkPhysicalDevice);

	VkSurfaceKHR GetSurface();
	const VkSurfaceCapabilitiesKHR& GetSurfaceCapabilities();
	const VkSurfaceFormatKHR& GetBestSurfaceFormat();
	const VkPresentModeKHR& GetBestPresentMode();
private:
	void CreateSurface();

	VkSurfaceFormatKHR ChooseBestSurfaceFormat();
	VkPresentModeKHR ChooseBestPresentMode();

	VkSurfaceKHR mSurface;
	VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
	VkSurfaceFormatKHR mBestSurfaceFormat;
	VkPresentModeKHR mBestPresentMode;

	std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;
	std::vector<VkPresentModeKHR> mAvailablePresentModes;

	WindowWrapper* mWindow;
	InstanceWrapper* mInstance;
};
#endif