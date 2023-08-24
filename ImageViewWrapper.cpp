#include "ImageViewWrapper.h"
#include "globals.h"
#include "Context.h"
#include "LogicalDeviceWrapper.h"


ImageViewWrapper::ImageViewWrapper(Context* context, VkImage image, VkFormat format, VkImageAspectFlags flags) : mContext(context) {
	CreateImageView(image, format, flags);
}

ImageViewWrapper::ImageViewWrapper(LogicalDeviceWrapper* lDevice, VkImage image, VkFormat format, VkImageAspectFlags flags) : mLogicalDevice(lDevice) {
	mContext = nullptr;
	CreateSwapchainImageView(image, format, flags);
}

ImageViewWrapper::~ImageViewWrapper() {
	if (mContext == nullptr) {
		vkDestroyImageView(mLogicalDevice->GetLogicalDevice(), mImageView, nullptr); std::cout << "Success: Image View destroyed." << std::endl;
	} else {
		vkDestroyImageView(mContext->mLogicalDevice->GetLogicalDevice(), mImageView, nullptr); std::cout << "Success: Image View destroyed." << std::endl;
	}
}

VkImageView ImageViewWrapper::GetImageView() {
	return mImageView;
}

void ImageViewWrapper::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags flags) {
	// Describe the ImageView
	VkComponentMapping imageViewComponentMapping = {
		.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.a = VK_COMPONENT_SWIZZLE_IDENTITY
	};
	VkImageSubresourceRange imageSubresourceRange = {
		.aspectMask = flags,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
	VkImageViewCreateInfo imageViewCI = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.image = image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
		.components = imageViewComponentMapping,
		.subresourceRange = imageSubresourceRange
	};

	// Create the ImageView
	VkResult result = vkCreateImageView(mContext->mLogicalDevice->GetLogicalDevice(), &imageViewCI, nullptr, &mImageView);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image View created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create an Image View! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void ImageViewWrapper::CreateSwapchainImageView(VkImage image, VkFormat format, VkImageAspectFlags flags) {
	// Describe the ImageView
	VkComponentMapping imageViewComponentMapping = {
		.r = VK_COMPONENT_SWIZZLE_IDENTITY,
		.g = VK_COMPONENT_SWIZZLE_IDENTITY,
		.b = VK_COMPONENT_SWIZZLE_IDENTITY,
		.a = VK_COMPONENT_SWIZZLE_IDENTITY
	};
	VkImageSubresourceRange imageSubresourceRange = {
		.aspectMask = flags,
		.baseMipLevel = 0,
		.levelCount = 1,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
	VkImageViewCreateInfo imageViewCI = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.image = image,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = format,
		.components = imageViewComponentMapping,
		.subresourceRange = imageSubresourceRange
	};

	// Create the ImageView
	VkResult result = vkCreateImageView(mLogicalDevice->GetLogicalDevice(), &imageViewCI, nullptr, &mImageView);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image View created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create an Image View! Error Code: " + NT_CHECK_RESULT(result));
	}
}