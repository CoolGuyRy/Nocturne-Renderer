#ifndef DESCRIPTOR_WRAPPER
#define DESCRIPTOR_WRAPPER

#include <vulkan/vulkan.h>

class Context;
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
	DescriptorSetLayoutWrapper(Context* , DESCRIPTOR_TYPE);
	~DescriptorSetLayoutWrapper();

	VkDescriptorSetLayout GetDescriptorSetLayout();
private:
	void CreateGenericDescriptorSetLayout();
	void CreateDynamicDescriptorSetLayout();
	void CreateTextureDescriptorSetLayout();

	VkDescriptorSetLayout mDescriptorSetLayout;

	Context* mContext;
};

class DescriptorPoolWrapper {
public:
	DescriptorPoolWrapper(Context*, DESCRIPTOR_TYPE);
	~DescriptorPoolWrapper();

	VkDescriptorPool GetDescriptorPool();
private:
	void CreateGenericDescriptorPool();
	void CreateDynamicDescriptorPool();
	void CreateTextureDescriptorPool();

	VkDescriptorPool mDescriptorPool;
	
	Context* mContext;
};

class DescriptorSetWrapper {
public:
	DescriptorSetWrapper(Context*, DescriptorSetLayoutWrapper*, DescriptorPoolWrapper*, DESCRIPTOR_TYPE);
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

	Context* mContext;
	DescriptorSetLayoutWrapper* mDescriptorSetLayout;
	DescriptorPoolWrapper* mDescriptorPool;
};
#endif