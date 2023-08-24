#include "SwapchainWrapper.h"
#include "globals.h"
#include "SurfaceWrapper.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "ImageViewWrapper.h"

SwapchainWrapper::SwapchainWrapper(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, SurfaceWrapper* surface) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice), mSurface(surface) {
	CreateSwapchain();
}

SwapchainWrapper::~SwapchainWrapper() {
	for (size_t i = 0; i < mSwapchainImages.size(); i++) {
		delete mSwapchainImages.at(i).mImageView;
	}
	vkDestroySwapchainKHR(mLogicalDevice->GetLogicalDevice(), mSwapchain, nullptr); std:: cout << "Success: Swapchain destroyed." << std::endl;
}

VkSwapchainKHR SwapchainWrapper::GetSwapchain() {
	return mSwapchain;
}

VkExtent2D SwapchainWrapper::GetSwapchainExtent() {
	return mSwapchainExtent;
}

std::vector<SwapchainImage> SwapchainWrapper::GetSwapchainImages() {
	return mSwapchainImages;
}

void SwapchainWrapper::CreateSwapchain() {
	// Ask Surface to Provide Swapchain Details
	mSurface->AcquireSurfaceProperties(mPhysicalDevice->GetPhysicalDevice());

	// Describe the swapchain
	VkSwapchainCreateInfoKHR swapchainCI = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = 0,
		.surface = mSurface->GetSurface(),
		.minImageCount = 3,
		.imageFormat = mSurface->GetBestSurfaceFormat().format,
		.imageColorSpace = mSurface->GetBestSurfaceFormat().colorSpace,
		.imageExtent = { 
			.width = WINDOW_WIDTH, 
			.height = WINDOW_HEIGHT 
		},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = mSurface->GetBestPresentMode(),
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE
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
		throw std::runtime_error("Failed to create swapchain! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Establish the Swapchain Extent
	mSwapchainExtent = {
		.width = (uint32_t)WINDOW_WIDTH,
		.height = (uint32_t)WINDOW_HEIGHT
	};

	// Grab the Swapchain Images
	uint32_t swapchainImagesCount = 0;
	vkGetSwapchainImagesKHR(mLogicalDevice->GetLogicalDevice(), mSwapchain, &swapchainImagesCount, nullptr);
	std::vector<VkImage> swapchainImages(swapchainImagesCount);
	vkGetSwapchainImagesKHR(mLogicalDevice->GetLogicalDevice(), mSwapchain, &swapchainImagesCount, swapchainImages.data());

	// Create ImageViews for each Swapchain Image and push to mSwapchainImages vector
	for (size_t i = 0; i < swapchainImages.size(); i++) {
		mSwapchainImages.push_back({swapchainImages.at(i), new ImageViewWrapper(mLogicalDevice, swapchainImages.at(i), mSurface->GetBestSurfaceFormat().format, VK_IMAGE_ASPECT_COLOR_BIT) });
	}
}