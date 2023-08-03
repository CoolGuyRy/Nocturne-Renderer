#ifndef IMAGE_VIEW_WRAPPER_H
#define IMAGE_VIEW_WRAPPER_H

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;

/*

	

*/

class ImageViewWrapper {
public:
	ImageViewWrapper(LogicalDeviceWrapper*, VkImage, VkFormat, VkImageAspectFlags);
	~ImageViewWrapper();

	VkImageView GetImageView();
private:
	void CreateImageView(VkImage, VkFormat, VkImageAspectFlags);

	VkImageView mImageView;

	LogicalDeviceWrapper* mLogicalDevice;
};

#endif