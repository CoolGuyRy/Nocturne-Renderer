#ifndef DESCRIPTOR_SET_WRAPPER
#define DESCRIPTOR_SET_WRAPPER

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class BufferWrapper;

class DescriptorSetLayoutWrapper {
public:
	DescriptorSetLayoutWrapper(LogicalDeviceWrapper*);
	~DescriptorSetLayoutWrapper();

	VkDescriptorSetLayout GetDescriptorSetLayout();
private:
	void CreateGenericDescriptorSetLayout();

	VkDescriptorSetLayout mDescriptorSetLayout;

	LogicalDeviceWrapper* mLogicalDeviceWrapper;
};

class DescriptorPoolWrapper {
public:
	DescriptorPoolWrapper(LogicalDeviceWrapper*);
	~DescriptorPoolWrapper();

	VkDescriptorPool GetDescriptorPool();
private:
	void CreateGenericDescriptorPool();

	VkDescriptorPool mDescriptorPool;
	
	LogicalDeviceWrapper* mLogicalDevice;
};

class DescriptorSetWrapper {
public:
	DescriptorSetWrapper(LogicalDeviceWrapper*, DescriptorSetLayoutWrapper*, DescriptorPoolWrapper*);
	~DescriptorSetWrapper();

	void WriteGenericDescriptorSet(BufferWrapper*);

	VkDescriptorSet GetDescriptorSet();
private:
	void CreateGenericDescriptorSet();

	VkDescriptorSet mDescriptorSet;

	LogicalDeviceWrapper* mLogicalDevice;
	DescriptorSetLayoutWrapper* mDescriptorSetLayout;
	DescriptorPoolWrapper* mDescriptorPool;
};
#endif