#ifndef IMAGE_VIEW_WRAPPER_H
#define IMAGE_VIEW_WRAPPER_H

#include <vulkan/vulkan.h>

class ImageView {
public:
	ImageView(VkDevice, VkImage, VkFormat, VkImageAspectFlags);
	~ImageView();

	VkImageView GetImageView();
private:
	VkImageView mImageView;
	VkDevice mLogicalDevice;
};

#endif