#include "DescriptorSetWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"
#include "BufferWrapper.h"

DescriptorSetLayoutWrapper::DescriptorSetLayoutWrapper(LogicalDeviceWrapper* lDevice) : mLogicalDeviceWrapper(lDevice) {
	CreateGenericDescriptorSetLayout();
}

DescriptorSetLayoutWrapper::~DescriptorSetLayoutWrapper() {
	vkDestroyDescriptorSetLayout(mLogicalDeviceWrapper->GetLogicalDevice(), mDescriptorSetLayout, nullptr); std::cout << "Success: Descriptor Set Layout destroyed." << std::endl;
}

VkDescriptorSetLayout DescriptorSetLayoutWrapper::GetDescriptorSetLayout() {
	return mDescriptorSetLayout;
}

void DescriptorSetLayoutWrapper::CreateGenericDescriptorSetLayout() {
	// MVP Binding Information
	VkDescriptorSetLayoutBinding mvpLayoutBinding = {
		0,											// binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,			// descriptorType
		1,											// descriptorCount
		VK_SHADER_STAGE_VERTEX_BIT,					// stageFlags
		nullptr										// pImmutableSamplers
	};

	// Create Descriptor Set Layout with given bindings
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,	// sType
		nullptr,												// pNext
		0,														// flags
		1,														// bindingCount
		&mvpLayoutBinding										// pBindings
	};

	VkResult result = vkCreateDescriptorSetLayout(mLogicalDeviceWrapper->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &mDescriptorSetLayout);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Set Layout created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Descriptor Set Layout! Error Code: " + NT_CHECK_RESULT(result));
	}
}

DescriptorPoolWrapper::DescriptorPoolWrapper(LogicalDeviceWrapper* lDevice) : mLogicalDevice(lDevice) {
	CreateGenericDescriptorPool();
}

DescriptorPoolWrapper::~DescriptorPoolWrapper() {
	vkDestroyDescriptorPool(mLogicalDevice->GetLogicalDevice(), mDescriptorPool, nullptr); std::cout << "Success: Descriptor Pool destroyed." << std::endl;
}

VkDescriptorPool DescriptorPoolWrapper::GetDescriptorPool() {
	return mDescriptorPool;
}

void DescriptorPoolWrapper::CreateGenericDescriptorPool() {
	VkDescriptorPoolSize poolSize = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,								// sType
		SWAPCHAIN_IMAGE_COUNT											// descriptorCount
	};

	VkDescriptorPoolCreateInfo descriptorPoolCI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,					// sType
		nullptr,														// pNext
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,				// flags
		SWAPCHAIN_IMAGE_COUNT,											// maxSets
		1,																// poolSizeCount
		&poolSize														// pPoolSizes
	};

	VkResult result = vkCreateDescriptorPool(mLogicalDevice->GetLogicalDevice(), &descriptorPoolCI, nullptr, &mDescriptorPool);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Pool created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Descriptor Pool! Error Code: " + NT_CHECK_RESULT(result));
	}
}

DescriptorSetWrapper::DescriptorSetWrapper(LogicalDeviceWrapper* lDevice , DescriptorSetLayoutWrapper* layout, DescriptorPoolWrapper* pool) : mLogicalDevice(lDevice), mDescriptorSetLayout(layout), mDescriptorPool(pool) {
	CreateGenericDescriptorSet();
}

DescriptorSetWrapper::~DescriptorSetWrapper() {
	vkFreeDescriptorSets(mLogicalDevice->GetLogicalDevice(), mDescriptorPool->GetDescriptorPool(), 1, &mDescriptorSet); std::cout << "Success: Descriptor Set freed." << std::endl;
}

void DescriptorSetWrapper::WriteGenericDescriptorSet(BufferWrapper* buffer) {
	VkDescriptorBufferInfo descriptorBI = {
		buffer->GetBuffer(),			// buffer
		0,								// offset
		// sizeof(MVP)						// range
		VK_WHOLE_SIZE					// range (TODO: TEST LATER)
	};

	VkWriteDescriptorSet writeDescriptorSet = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,		// sType
		nullptr,									// pNext
		mDescriptorSet,								// dstSet
		0,											// dstBinding
		0,											// dstArrayElement
		1,											// descriptorCount
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,			// descriptorType
		nullptr,									// pImageInfo
		&descriptorBI,								// pBufferInfo
		nullptr										// pTexelBufferView
	};

	vkUpdateDescriptorSets(mLogicalDevice->GetLogicalDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

VkDescriptorSet DescriptorSetWrapper::GetDescriptorSet() {
	return mDescriptorSet;
}

void DescriptorSetWrapper::CreateGenericDescriptorSet() {
	VkDescriptorSetLayout layout = mDescriptorSetLayout->GetDescriptorSetLayout();
	VkDescriptorSetAllocateInfo descriptorSetAI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,		// sType
		nullptr,											// pNext
		mDescriptorPool->GetDescriptorPool(),				// descriptorPool
		1,													// descriptorSetCount
		&layout												// pSetLayouts
	};

	VkResult result = vkAllocateDescriptorSets(mLogicalDevice->GetLogicalDevice(), &descriptorSetAI, &mDescriptorSet);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Set allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Descriptor Set! Error Code: " + NT_CHECK_RESULT(result));
	}
}