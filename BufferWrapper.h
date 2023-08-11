#ifndef BUFFER_WRAPPER_H
#define BUFFER_WRAPPER_H

#include <iostream>
#include <vulkan/vulkan.h>

class PhysicalDeviceWrapper;
class LogicalDeviceWrapper;
class CommandPoolWrapper;

/*

	

*/

class BufferWrapper {
public:
	BufferWrapper(PhysicalDeviceWrapper*, LogicalDeviceWrapper*, VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);
	~BufferWrapper();

	void MapBufferMemory(void* data, VkDeviceSize dSize);

	VkBuffer GetBuffer();
	VkDeviceMemory GetBufferMemory();
private:
	void CreateBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);

	uint32_t FindMemoryTypeIndex(uint32_t, VkMemoryPropertyFlags);

	VkBuffer mBuffer;
	VkDeviceMemory mBufferMemory;

	PhysicalDeviceWrapper* mPhysicalDevice;
	LogicalDeviceWrapper* mLogicalDevice;
};

void CopyBuffer(LogicalDeviceWrapper*, CommandPoolWrapper*, BufferWrapper*, BufferWrapper*, VkDeviceSize);

#endif