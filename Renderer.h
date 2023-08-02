#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

class WindowWrapper;
class InstanceWrapper;
class SurfaceWrapper;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class SwapchainWrapper;

class Renderer {
public:
	Renderer(WindowWrapper*);
	~Renderer();
private:
	WindowWrapper* mWindow;
	InstanceWrapper* mInstance;
	SurfaceWrapper* mSurface;
	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SwapchainWrapper* mSwapchain;
};
#endif