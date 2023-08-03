#include "ImageViewWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"


ImageViewWrapper::ImageViewWrapper(LogicalDeviceWrapper* lDevice, VkImage image, VkFormat format, VkImageAspectFlags flags) : mLogicalDevice(lDevice) {
	CreateImageView(image, format, flags);
}

ImageViewWrapper::~ImageViewWrapper() {
	vkDestroyImageView(mLogicalDevice->GetLogicalDevice(), mImageView, nullptr); std::cout << "Success: Image View destroyed." << std::endl;
}

VkImageView ImageViewWrapper::GetImageView() {
	return mImageView;
}

void ImageViewWrapper::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags flags) {
	// Describe the ImageView
	VkComponentMapping imageViewComponentMapping = {
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY
	};
	VkImageSubresourceRange imageSubresourceRange = {
		flags,										// aspectMask
		0,											// baseMipLevel
		1,											// levelCount
		0,											// baseArrayLayer
		1											// layerCount
	};
	VkImageViewCreateInfo imageViewCI = {
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	// sType
		nullptr,									// pNext
		0,											// flags
		image,										// image
		VK_IMAGE_VIEW_TYPE_2D,						// viewType
		format,										// format
		imageViewComponentMapping,					// components
		imageSubresourceRange						// subresourceRange
	};

	// Create the ImageView
	VkResult result = vkCreateImageView(mLogicalDevice->GetLogicalDevice(), &imageViewCI, nullptr, &mImageView);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image View created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create an Image View! Error Code: " + NT_CHECK_RESULT(result));
	}
}