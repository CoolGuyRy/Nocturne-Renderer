#include "SwapchainWrapper.h"
#include "globals.h"
#include "SurfaceWrapper.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"

SwapchainWrapper::SwapchainWrapper(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, SurfaceWrapper* surface) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice), mSurface(surface) {
	CreateSwapchain();
}

SwapchainWrapper::~SwapchainWrapper() {
	vkDestroySwapchainKHR(mLogicalDevice->GetLogicalDevice(), mSwapchain, nullptr); std:: cout << "Success: Swapchain destroyed." << std::endl;
}

void SwapchainWrapper::CreateSwapchain() {
	// Describe the swapchain
	VkSwapchainCreateInfoKHR swapchainCI = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,					// sType
		nullptr,														// pNext
		0,																// flags
		mSurface->GetSurface(),											// surface
		3,																// minImageCount
		VK_FORMAT_B8G8R8A8_UNORM,										// imageFormat
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,								// imageColorSpace
		{ WINDOW_WIDTH, WINDOW_HEIGHT },								// imageExtent
		1,																// imageArrayLayers
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,							// imageUsage
		VK_SHARING_MODE_EXCLUSIVE,										// imageSharingMode
		0,																// queueFamilyIndexCount
		nullptr,														// pQueueFamilyIndices
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,							// preTransform
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,								// compositeAlpha
		VK_PRESENT_MODE_FIFO_KHR,										// presentMode
		VK_TRUE,														// clipped
		VK_NULL_HANDLE													// oldSwapchain
	};

	// Check if the queue families are the same
	std::vector<uint32_t> queueFamilyIndices;
	queueFamilyIndices.push_back(mPhysicalDevice->GetQueueFamilyIndices().mGraphics);
	if (mPhysicalDevice->GetQueueFamilyIndices().mGraphics == mPhysicalDevice->GetQueueFamilyIndices().mPresent) {
		swapchainCI.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();
		swapchainCI.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		queueFamilyIndices.push_back(mPhysicalDevice->GetQueueFamilyIndices().mPresent);
		swapchainCI.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();
		swapchainCI.pQueueFamilyIndices = queueFamilyIndices.data();
		swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	}

	// Create the swapchain
	VkResult result = vkCreateSwapchainKHR(mLogicalDevice->GetLogicalDevice(), &swapchainCI, nullptr, &mSwapchain);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Swapchain created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create swapchain!");
	}
}
