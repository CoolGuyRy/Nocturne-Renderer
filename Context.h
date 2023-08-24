#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class WindowWrapper;
class InstanceWrapper;
class SurfaceWrapper;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class SwapchainWrapper;
class RenderPassWrapper;

struct QueueFamilyIndices;

class Context {
public:
	Context();
	~Context();

	WindowWrapper* mWindow;
	InstanceWrapper* mInstance;
	SurfaceWrapper* mSurface;
	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SwapchainWrapper* mSwapchain;
	RenderPassWrapper* mRenderPass;
private:

};

#endif