#include "Renderer.h"

Renderer::Renderer() {
	mInstance = new InstanceWrapper();
	mPhysicalDevice = new PhysicalDeviceWrapper(mInstance->GetInstance());
}

Renderer::~Renderer() {
	// Don't forget to insert in reverse order
	delete mPhysicalDevice;
	delete mInstance;
}