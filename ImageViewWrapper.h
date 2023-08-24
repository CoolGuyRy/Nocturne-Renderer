#ifndef IMAGE_VIEW_WRAPPER_H
#define IMAGE_VIEW_WRAPPER_H

#include <vulkan/vulkan.h>

class Context;
class LogicalDeviceWrapper;

/*

	

*/

class ImageViewWrapper {
public:
	ImageViewWrapper(Context*, VkImage, VkFormat, VkImageAspectFlags);
	ImageViewWrapper(LogicalDeviceWrapper*, VkImage, VkFormat, VkImageAspectFlags);
	~ImageViewWrapper();

	VkImageView GetImageView();
private:
	void CreateImageView(VkImage, VkFormat, VkImageAspectFlags);
	void CreateSwapchainImageView(VkImage, VkFormat, VkImageAspectFlags);

	VkImageView mImageView;

	Context* mContext;
	LogicalDeviceWrapper* mLogicalDevice;
};

#endif