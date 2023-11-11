#include "ImageView.h"

#include <iostream>
#include "globals.h"

ImageView::ImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags flags) : mLogicalDevice(device) {
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
	VkResult result = vkCreateImageView(mLogicalDevice, &imageViewCI, nullptr, &mImageView);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image View created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create an Image View! Error Code: " + NT_CHECK_RESULT(result));
	}
}

ImageView::~ImageView() {
	vkDestroyImageView(mLogicalDevice, mImageView, nullptr); std::cout << "Success: Image View destroyed." << std::endl;
}

VkImageView ImageView::GetImageView() {
	return mImageView;
}