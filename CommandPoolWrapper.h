#ifndef COMMAND_POOL_WRAPPER
#define COMMAND_POOL_WRAPPER

#include <vulkan/vulkan.h>

class Context;

/*

	

*/

class CommandPoolWrapper {
public:
	CommandPoolWrapper(Context*, uint32_t);
	~CommandPoolWrapper();

	VkCommandPool GetCommandPool();
private:
	void CreateCommandPool(uint32_t);

	VkCommandPool mCommandPool;

	Context* mContext;
};

#endif
