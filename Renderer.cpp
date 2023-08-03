#include "Renderer.h"
#include "WindowWrapper.h"
#include "InstanceWrapper.h"
#include "SurfaceWrapper.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "SwapchainWrapper.h"
#include "ShaderWrapper.h"
#include "RenderPassWrapper.h"
#include "PipelineWrapper.h"

Renderer::Renderer(WindowWrapper* window) : mWindow(window) {
	mInstance = new InstanceWrapper();
	mSurface = new SurfaceWrapper(mWindow, mInstance);
	mPhysicalDevice = new PhysicalDeviceWrapper(mInstance, mSurface);
	mLogicalDevice = new LogicalDeviceWrapper(mPhysicalDevice);
	mSwapchain = new SwapchainWrapper(mPhysicalDevice, mLogicalDevice, mSurface);
	mRenderPass = new RenderPassWrapper(mLogicalDevice, mSurface);
	mPipeline = new PipelineWrapper(mLogicalDevice, mRenderPass);
}

Renderer::~Renderer() {
	vkDeviceWaitIdle(mLogicalDevice->GetLogicalDevice());

	// Don't forget to insert in reverse order
	delete mPipeline;
	delete mRenderPass;
	delete mSwapchain;
	delete mLogicalDevice;
	delete mPhysicalDevice;
	delete mSurface;
	delete mInstance;
}