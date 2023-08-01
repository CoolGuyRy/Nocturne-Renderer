#include "Renderer.h"

Renderer::Renderer() {
	mInstance = new InstanceWrapper();
	mPhysicalDevice = new PhysicalDeviceWrapper(mInstance);
	mLogicalDevice = new LogicalDeviceWrapper(mPhysicalDevice);
}

Renderer::~Renderer() {
	// Don't forget to insert in reverse order
	delete mLogicalDevice;
	delete mPhysicalDevice;
	delete mInstance;
}