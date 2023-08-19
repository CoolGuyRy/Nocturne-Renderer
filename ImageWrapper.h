#ifndef IMAGE_WRAPPER_H
#define IMAGE_WRAPPER_H

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class PhysicalDeviceWrapper;

class ImageWrapper {
public:
	ImageWrapper(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags);
	~ImageWrapper();

	VkImage GetImage();
private:
	void CreateDepthImage(uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags);

	VkImage mImage;
	VkDeviceMemory mImageMemory;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
};

#endif