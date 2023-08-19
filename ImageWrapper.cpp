#include "ImageWrapper.h"
#include "globals.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"

ImageWrapper::ImageWrapper(PhysicalDeviceWrapper* pDevice, LogicalDeviceWrapper* lDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags) : mPhysicalDevice(pDevice), mLogicalDevice(lDevice) {
	CreateDepthImage(width, height, format, tiling, useFlags, propFlags);
}

ImageWrapper::~ImageWrapper() {
	vkDestroyImage(mLogicalDevice->GetLogicalDevice(), mImage, nullptr);
	vkFreeMemory(mLogicalDevice->GetLogicalDevice(), mImageMemory, nullptr);
}

VkImage ImageWrapper::GetImage() {
	return mImage;
}

void ImageWrapper::CreateDepthImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags) {
	VkImageCreateInfo imageCI = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,																		// sType
		nullptr,																									// pNext
		0,																											// flags
		VK_IMAGE_TYPE_2D,																							// imageType
		format,																										// format
		{
			width,																									// extent width
			height,																									// extent height
			1																										// extent depth
		},																											// extent
		1,																											// mipLevels
		1,																											// arrayLayers
		VK_SAMPLE_COUNT_1_BIT,																						// samples
		tiling,																										// tiling
		useFlags,																									// usage
		VK_SHARING_MODE_EXCLUSIVE,																					// sharingMode
		0,																											// queueFamilyIndexCount
		nullptr,																									// pQueueFamilyIndices
		VK_IMAGE_LAYOUT_UNDEFINED																					// initialLayout
	};

	VkResult result = vkCreateImage(mLogicalDevice->GetLogicalDevice(), &imageCI, nullptr, &mImage);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Image! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(mLogicalDevice->GetLogicalDevice(), mImage, &memoryRequirements);

	VkMemoryAllocateInfo memoryAI = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,																		// sType
		nullptr,																									// pNext
		memoryRequirements.size,																					// allocationSize
		FindMemoryTypeIndex(mPhysicalDevice->GetPhysicalDevice(), memoryRequirements.memoryTypeBits, propFlags)		// memoryTypeIndex
	};

	result = vkAllocateMemory(mLogicalDevice->GetLogicalDevice(), &memoryAI, nullptr, &mImageMemory);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Image Memory allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate image memory! Error Code: " + NT_CHECK_RESULT(result));
	}

	vkBindImageMemory(mLogicalDevice->GetLogicalDevice(), mImage, mImageMemory, 0);
}