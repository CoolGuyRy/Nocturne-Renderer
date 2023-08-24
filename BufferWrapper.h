#ifndef BUFFER_WRAPPER_H
#define BUFFER_WRAPPER_H

#include <iostream>
#include <vulkan/vulkan.h>

class Context;
class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class CommandPoolWrapper;
class ImageWrapper;

/*

	

*/

class BufferWrapper {
public:
	BufferWrapper(Context*, VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);
	~BufferWrapper();

	void MapBufferMemory(void* data, VkDeviceSize dSize);

	VkBuffer GetBuffer();
	VkDeviceMemory GetBufferMemory();
private:
	void CreateBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);

	VkBuffer mBuffer;
	VkDeviceMemory mBufferMemory;

	Context* mContext;
};

void CopyBuffer(Context*, CommandPoolWrapper*, BufferWrapper*, BufferWrapper*, VkDeviceSize);
#endif