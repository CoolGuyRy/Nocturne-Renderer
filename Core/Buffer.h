#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <vulkan/vulkan.h>

class Buffer {
public:
	Buffer(VkPhysicalDevice, VkDevice, VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);
	~Buffer();

	void MapBufferMemory(void* data, VkDeviceSize dSize);

	VkBuffer GetBuffer();
	VkDeviceMemory GetBufferMemory();
private:
	void CreateBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);

	VkBuffer mBuffer;
	VkDeviceMemory mBufferMemory;

	VkPhysicalDevice mPhysicalDevice;
	VkDevice mLogicalDevice;
};

void CopyBuffer(VkDevice mLogicalDevice, VkCommandPool pool, VkQueue transferQueue, Buffer* srcBuffer, Buffer* dstBuffer, VkDeviceSize bufferSize);
#endif