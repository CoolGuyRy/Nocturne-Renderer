#include "Context.h"
#include "globals.h"
#include "WindowWrapper.h"
#include "InstanceWrapper.h"
#include "SurfaceWrapper.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "SwapchainWrapper.h"
#include "RenderPassWrapper.h"

Context::Context() {
	mWindow = new WindowWrapper();

	mInstance = new InstanceWrapper();

	mSurface = new SurfaceWrapper(mWindow, mInstance);

	mPhysicalDevice = new PhysicalDeviceWrapper(mInstance, mSurface);

	mLogicalDevice = new LogicalDeviceWrapper(mPhysicalDevice);

	mSwapchain = new SwapchainWrapper(mPhysicalDevice, mLogicalDevice, mSurface);

	mRenderPass = new RenderPassWrapper(mLogicalDevice, mSurface);
}

Context::~Context() {
	// Delete in reverse order!
	delete mRenderPass;
	delete mSwapchain;
	delete mLogicalDevice;
	delete mPhysicalDevice;
	delete mSurface;
	delete mInstance;
	delete mWindow;
}