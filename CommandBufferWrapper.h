#ifndef COMMAND_BUFFER_WRAPPER
#define COMMAND_BUFFER_WRAPPER

#include <vulkan/vulkan.h>

class Context;
class LogicalDeviceWrapper;
class CommandPoolWrapper;

/*

	

*/

class CommandBufferWrapper {
public:
	CommandBufferWrapper(Context*, CommandPoolWrapper*);
	~CommandBufferWrapper();

	VkCommandBuffer GetCommandBuffer();
private:
	void AllocateCommandBuffer();

	VkCommandBuffer mCommandBuffer;

	Context* mContext;
	CommandPoolWrapper* mCommandPool;
};

#endif
