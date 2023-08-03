#ifndef COMMAND_BUFFER_WRAPPER
#define COMMAND_BUFFER_WRAPPER

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class CommandPoolWrapper;

/*

	

*/

class CommandBufferWrapper {
public:
	CommandBufferWrapper(LogicalDeviceWrapper*, CommandPoolWrapper*);
	~CommandBufferWrapper();

	VkCommandBuffer GetCommandBuffer();
private:
	void AllocateCommandBuffer();

	VkCommandBuffer mCommandBuffer;

	LogicalDeviceWrapper* mLogicalDevice;
	CommandPoolWrapper* mCommandPool;
};

#endif
