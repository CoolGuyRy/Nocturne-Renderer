#ifndef COMMAND_POOL_WRAPPER
#define COMMAND_POOL_WRAPPER

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;

/*

	

*/

class CommandPoolWrapper {
public:
	CommandPoolWrapper(LogicalDeviceWrapper*, uint32_t);
	~CommandPoolWrapper();

	VkCommandPool GetCommandPool();
private:
	void CreateCommandPool(uint32_t);

	VkCommandPool mCommandPool;

	LogicalDeviceWrapper* mLogicalDevice;
};

#endif
