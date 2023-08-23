#ifndef DESCRIPTOR_SET_WRAPPER
#define DESCRIPTOR_SET_WRAPPER

#include <vulkan/vulkan.h>

class LogicalDeviceWrapper;
class BufferWrapper;
class ImageViewWrapper;
class SamplerWrapper;

enum DESCRIPTOR_TYPE {
	GENERIC,
	DYNAMIC,
	TEXTURE
};

class DescriptorSetLayoutWrapper {
public:
	DescriptorSetLayoutWrapper(LogicalDeviceWrapper*, DESCRIPTOR_TYPE);
	~DescriptorSetLayoutWrapper();

	VkDescriptorSetLayout GetDescriptorSetLayout();
private:
	void CreateGenericDescriptorSetLayout();
	void CreateDynamicDescriptorSetLayout();
	void CreateTextureDescriptorSetLayout();

	VkDescriptorSetLayout mDescriptorSetLayout;

	LogicalDeviceWrapper* mLogicalDeviceWrapper;
};

class DescriptorPoolWrapper {
public:
	DescriptorPoolWrapper(LogicalDeviceWrapper*, DESCRIPTOR_TYPE);
	~DescriptorPoolWrapper();

	VkDescriptorPool GetDescriptorPool();
private:
	void CreateGenericDescriptorPool();
	void CreateDynamicDescriptorPool();
	void CreateTextureDescriptorPool();

	VkDescriptorPool mDescriptorPool;
	
	LogicalDeviceWrapper* mLogicalDevice;
};

class DescriptorSetWrapper {
public:
	DescriptorSetWrapper(LogicalDeviceWrapper*, DescriptorSetLayoutWrapper*, DescriptorPoolWrapper*, DESCRIPTOR_TYPE);
	~DescriptorSetWrapper();

	void WriteGenericDescriptorSet(BufferWrapper*);
	void WriteDynamicDescriptorSet(BufferWrapper*, BufferWrapper*);
	void WriteTextureDescriptorSet(ImageViewWrapper*, SamplerWrapper*);

	VkDescriptorSet GetDescriptorSet();
private:
	void CreateGenericDescriptorSet();
	void CreateDynamicDescriptorSet();
	void CreateTextureDescriptorSet();

	VkDescriptorSet mDescriptorSet;

	LogicalDeviceWrapper* mLogicalDevice;
	DescriptorSetLayoutWrapper* mDescriptorSetLayout;
	DescriptorPoolWrapper* mDescriptorPool;
};
#endif