#include "SurfaceWrapper.h"
#include "globals.h"
#include "WindowWrapper.h"
#include "InstanceWrapper.h"

SurfaceWrapper::SurfaceWrapper(WindowWrapper* window, InstanceWrapper* instance) : mWindow(window), mInstance(instance) {
	CreateSurface();
}

SurfaceWrapper::~SurfaceWrapper() {
	vkDestroySurfaceKHR(mInstance->GetInstance(), mSurface, nullptr); std::cout << "Success: Surface destroyed." << std::endl;
}

VkSurfaceKHR SurfaceWrapper::GetSurface() {
	return mSurface;
}

const VkSurfaceCapabilitiesKHR& SurfaceWrapper::GetAvailableSurfaceCapabilities() {
	return mSurfaceCapabilities;
}

const std::vector<VkSurfaceFormatKHR>& SurfaceWrapper::GetAvailableSurfaceFormats() {
	return mAvailableSurfaceFormats;
}

const std::vector<VkPresentModeKHR>& SurfaceWrapper::GetAvailablePresentModes() {
	return mAvailablePresentModes;
}

void SurfaceWrapper::CreateSurface() {
	// GLFW has a function to handle surface creation. So run function and check for errors.
	VkResult result = glfwCreateWindowSurface(mInstance->GetInstance(), mWindow->GetWindow(), nullptr, &mSurface);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Surface created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create surface! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void SurfaceWrapper::AcquireSurfaceProperties(VkPhysicalDevice pDevice) {
	// Get surface properties
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, mSurface, &mSurfaceCapabilities);

	// Get surface formats
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, mSurface, &formatCount, nullptr);
	mAvailableSurfaceFormats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, mSurface, &formatCount, mAvailableSurfaceFormats.data());

	// Get surface present modes
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, mSurface, &presentModeCount, nullptr);
	mAvailablePresentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, mSurface, &presentModeCount, mAvailablePresentModes.data());
}