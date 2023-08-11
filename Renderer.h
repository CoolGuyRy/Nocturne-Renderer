#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

#include "Mesh.h"

class WindowWrapper;
class InstanceWrapper;
class SurfaceWrapper;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class SwapchainWrapper;
class ShaderWrapper;
class RenderPassWrapper;
class PipelineWrapper;
class FramebufferWrapper;
class CommandPoolWrapper;
class CommandBufferWrapper;
class SemaphoreWrapper;
class FenceWrapper;

/*

	

*/

class Renderer {
public:
	Renderer(WindowWrapper*);
	~Renderer();

	void Draw();
private:
	void RecordCommands();

	Mesh* mFirstMesh;

	int mCurrentFrame;

	WindowWrapper* mWindow;
	InstanceWrapper* mInstance;
	SurfaceWrapper* mSurface;
	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	SwapchainWrapper* mSwapchain;
	RenderPassWrapper* mRenderPass;
	PipelineWrapper* mPipeline;
	std::vector<FramebufferWrapper*> mFramebuffers;
	CommandPoolWrapper* mGraphicsCommandPool;
	CommandPoolWrapper* mTransferCommandPool;
	std::vector<CommandBufferWrapper*> mCommandBuffers;
	std::vector<SemaphoreWrapper*> mImageAvailableSemaphores;
	std::vector<SemaphoreWrapper*> mRenderFinishedSemaphores;
	std::vector<FenceWrapper*> mDrawFences;
};
#endif
