#ifndef IMAGE_WRAPPER_H
#define IMAGE_WRAPPER_H

#include "stb_image.h"
#include <vulkan/vulkan.h>
#include <string>

class LogicalDeviceWrapper;
class PhysicalDeviceWrapper;
class CommandPoolWrapper;
class BufferWrapper;

/*

	Note: There is a problem with the CopyImageBuffer / TransitionImageLayout functions that I fixed
	in a hacky way once again. The queues and command pools are getting mixed up in the parameters.
	Need to sort that out at some point in time. As of now, just using the graphics queue works since
	it is all purpose. But should really fix it later.

*/

class ImageWrapper {
public:
	ImageWrapper(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, CommandPoolWrapper*, uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags);
	ImageWrapper(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, CommandPoolWrapper*, std::string);
	~ImageWrapper();

	VkImage GetImage();
private:
	void CreateImage(uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags);
	void CreateTextureImage(std::string filename);

	VkImage mImage;
	VkDeviceMemory mImageMemory;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
	CommandPoolWrapper* mGraphicsCommandPool;
};

stbi_uc* LoadTextureFile(std::string filename, int* width, int* height, VkDeviceSize* imageSize);
void CopyImageBuffer(LogicalDeviceWrapper*, CommandPoolWrapper*, BufferWrapper*, ImageWrapper*, uint32_t, uint32_t);
void TransitionImageLayout(LogicalDeviceWrapper*, CommandPoolWrapper*, ImageWrapper*, VkImageLayout, VkImageLayout);
#endif