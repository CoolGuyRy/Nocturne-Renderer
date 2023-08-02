#ifndef SURFACE_WRAPPER_H
#define SURFACE_WRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

class WindowWrapper;
class InstanceWrapper;

class SurfaceWrapper {
public:
	SurfaceWrapper(WindowWrapper*, InstanceWrapper*);
	~SurfaceWrapper();

	void AcquireSurfaceProperties(VkPhysicalDevice);

	VkSurfaceKHR GetSurface();
	const VkSurfaceCapabilitiesKHR& GetAvailableSurfaceCapabilities();
	const std::vector<VkSurfaceFormatKHR>& GetAvailableSurfaceFormats();
	const std::vector<VkPresentModeKHR>& GetAvailablePresentModes();
private:
	void CreateSurface();

	VkSurfaceKHR mSurface;
	VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;
	std::vector<VkPresentModeKHR> mAvailablePresentModes;

	WindowWrapper* mWindow;
	InstanceWrapper* mInstance;
};
#endif