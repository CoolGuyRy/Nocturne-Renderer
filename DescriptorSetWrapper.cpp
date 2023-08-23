#include "DescriptorSetWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"
#include "BufferWrapper.h"
#include "ImageViewWrapper.h"
#include "SamplerWrapper.h"

DescriptorSetLayoutWrapper::DescriptorSetLayoutWrapper(LogicalDeviceWrapper* lDevice, DESCRIPTOR_TYPE type) : mLogicalDeviceWrapper(lDevice) {
	switch (type) {
		case GENERIC:
			CreateGenericDescriptorSetLayout();
			break;
		case DYNAMIC:
			CreateDynamicDescriptorSetLayout();
			break;
		case TEXTURE:
			CreateTextureDescriptorSetLayout();
			break;
		default:
			throw std::runtime_error("Designated an incorrect DESCRIPTOR_TYPE value!");
	}
}

DescriptorSetLayoutWrapper::~DescriptorSetLayoutWrapper() {
	vkDestroyDescriptorSetLayout(mLogicalDeviceWrapper->GetLogicalDevice(), mDescriptorSetLayout, nullptr); std::cout << "Success: Descriptor Set Layout destroyed." << std::endl;
}

VkDescriptorSetLayout DescriptorSetLayoutWrapper::GetDescriptorSetLayout() {
	return mDescriptorSetLayout;
}

/*

	Creates a simple DescriptorSetLayout that only has a uniform binding.

*/
void DescriptorSetLayoutWrapper::CreateGenericDescriptorSetLayout() {
	// MVP Binding Information
	VkDescriptorSetLayoutBinding mvpLayoutBinding = {
		0,																// binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,								// descriptorType
		1,																// descriptorCount
		VK_SHADER_STAGE_VERTEX_BIT,										// stageFlags
		nullptr															// pImmutableSamplers
	};

	// Create Descriptor Set Layout with given bindings
	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,			// sType
		nullptr,														// pNext
		0,																// flags
		1,																// bindingCount
		&mvpLayoutBinding												// pBindings
	};

	VkResult result = vkCreateDescriptorSetLayout(mLogicalDeviceWrapper->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &mDescriptorSetLayout);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Generic Descriptor Set Layout created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Generic Descriptor Set Layout! Error Code: " + NT_CHECK_RESULT(result));
	}
}

/*

	Creates a DescriptorSetLayout that has support for both a uniform binding and a dynamic uniform binding

*/
void DescriptorSetLayoutWrapper::CreateDynamicDescriptorSetLayout() {
	// Uniform Descriptor Set Layout Binding
	VkDescriptorSetLayoutBinding viewProjectionLayoutBinding = {
		0,																// binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,								// descriptorType
		1,																// descriptorCount
		VK_SHADER_STAGE_VERTEX_BIT,										// stageFlags
		nullptr															// pImmutableSamplers
	};

	// Dynamic Uniform Descriptor Set Layout Binding
	VkDescriptorSetLayoutBinding modelLayoutBinding = {
		1,																// binding
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,						// descriptorType
		1,																// descriptorCount
		VK_SHADER_STAGE_VERTEX_BIT,										// stageFlags
		nullptr															// pImmutableSamplers
	};

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings = { viewProjectionLayoutBinding, modelLayoutBinding };

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,			// sType
		nullptr,														// pNext
		0,																// flags
		(uint32_t)layoutBindings.size(),								// bindingCount
		layoutBindings.data()											// pBindings
	};

	VkResult result = vkCreateDescriptorSetLayout(mLogicalDeviceWrapper->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &mDescriptorSetLayout);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Dynamic Descriptor Set Layout created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Dynamic Descriptor Set Layout! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void DescriptorSetLayoutWrapper::CreateTextureDescriptorSetLayout() {
	// Texture Sampler Descriptor Set Layout Binding
	VkDescriptorSetLayoutBinding textureSamplerLayoutBinding = {
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
		.pImmutableSamplers = nullptr
	};

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCI = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.bindingCount = 1,
		.pBindings = &textureSamplerLayoutBinding
	};

	VkResult result = vkCreateDescriptorSetLayout(mLogicalDeviceWrapper->GetLogicalDevice(), &descriptorSetLayoutCI, nullptr, &mDescriptorSetLayout);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Texture Descriptor Set Layout created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Texture Descriptor Set! Error Code: " + NT_CHECK_RESULT(result));
	}
}

DescriptorPoolWrapper::DescriptorPoolWrapper(LogicalDeviceWrapper* lDevice, DESCRIPTOR_TYPE type) : mLogicalDevice(lDevice) {
	switch (type) {
	case GENERIC:
		CreateGenericDescriptorPool();
		break;
	case DYNAMIC:
		CreateDynamicDescriptorPool();
		break;
	case TEXTURE:
		CreateTextureDescriptorPool();
		break;
	default:
		throw std::runtime_error("Designated an incorrect DESCRIPTOR_TYPE value!");
	}
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

void DescriptorPoolWrapper::CreateDynamicDescriptorPool() {
	VkDescriptorPoolSize uniformPoolSize = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,								// sType
		SWAPCHAIN_IMAGE_COUNT											// descriptorCount
	};

	VkDescriptorPoolSize dynamicPoolSize = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,						// sType
		SWAPCHAIN_IMAGE_COUNT											// descriptorCount
	};

	std::vector<VkDescriptorPoolSize> poolSizes = { uniformPoolSize, dynamicPoolSize };

	VkDescriptorPoolCreateInfo descriptorPoolCI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,					// sType
		nullptr,														// pNext
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,				// flags
		SWAPCHAIN_IMAGE_COUNT,											// maxSets
		(uint32_t)poolSizes.size(),										// poolSizeCount
		poolSizes.data()												// pPoolSizes
	};

	VkResult result = vkCreateDescriptorPool(mLogicalDevice->GetLogicalDevice(), &descriptorPoolCI, nullptr, &mDescriptorPool);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Pool created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Descriptor Pool! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void DescriptorPoolWrapper::CreateTextureDescriptorPool() {
	VkDescriptorPoolSize texturePoolSize = {
		.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = MAX_OBJECTS
	};

	VkDescriptorPoolCreateInfo texturePoolCI = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.pNext = nullptr,
		.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		.maxSets = MAX_OBJECTS,
		.poolSizeCount = 1,
		.pPoolSizes = &texturePoolSize
	};

	VkResult result = vkCreateDescriptorPool(mLogicalDevice->GetLogicalDevice(), &texturePoolCI, nullptr, &mDescriptorPool);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Texture Descriptor Pool created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Texture Descriptor Pool! Error Code: " + NT_CHECK_RESULT(result));
	}
}

DescriptorSetWrapper::DescriptorSetWrapper(LogicalDeviceWrapper* lDevice , DescriptorSetLayoutWrapper* layout, DescriptorPoolWrapper* pool, DESCRIPTOR_TYPE type) : mLogicalDevice(lDevice), mDescriptorSetLayout(layout), mDescriptorPool(pool) {
	switch (type) {
		case GENERIC:
			CreateGenericDescriptorSet();
			break;
		case DYNAMIC:
			CreateDynamicDescriptorSet();
			break;
		case TEXTURE:
			CreateTextureDescriptorSet();
			break;
	}
}

DescriptorSetWrapper::~DescriptorSetWrapper() {
	vkFreeDescriptorSets(mLogicalDevice->GetLogicalDevice(), mDescriptorPool->GetDescriptorPool(), 1, &mDescriptorSet); std::cout << "Success: Descriptor Set freed." << std::endl;
}

void DescriptorSetWrapper::WriteGenericDescriptorSet(BufferWrapper* buffer) {
	VkDescriptorBufferInfo descriptorBI = {
		buffer->GetBuffer(),											// buffer
		0,																// offset
		VK_WHOLE_SIZE													// range
	};

	VkWriteDescriptorSet writeDescriptorSet = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,							// sType
		nullptr,														// pNext
		mDescriptorSet,													// dstSet
		0,																// dstBinding
		0,																// dstArrayElement
		1,																// descriptorCount
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,								// descriptorType
		nullptr,														// pImageInfo
		&descriptorBI,													// pBufferInfo
		nullptr															// pTexelBufferView
	};

	vkUpdateDescriptorSets(mLogicalDevice->GetLogicalDevice(), 1, &writeDescriptorSet, 0, nullptr);
}

void DescriptorSetWrapper::WriteDynamicDescriptorSet(BufferWrapper* viewProj, BufferWrapper* model) {
	VkDescriptorBufferInfo viewProjBI = {
		viewProj->GetBuffer(),											// buffer
		0,																// offset
		VK_WHOLE_SIZE,													// range
	};

	VkWriteDescriptorSet viewProjWriteDescriptorSet = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,							// sType
		nullptr,														// pNext
		mDescriptorSet,													// dstSet
		0,																// dstBinding
		0,																// dstArrayElement
		1,																// descriptorCount
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,								// descriptorType
		nullptr,														// pImageInfo
		&viewProjBI,													// pBufferInfo
		nullptr															// pTexelBufferView
	};

	VkDescriptorBufferInfo modelBI = {
		model->GetBuffer(),												// buffer
		0,																// offset
		sizeof(glm::mat4)												// range (NOTE: WRONG WAY TO DO THIS. SHOULD GRAB ALIGN VALUE FROM RENDERER)
	};

	VkWriteDescriptorSet modelWriteDescriptorSet = {
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,							// sType
		nullptr,														// pNext
		mDescriptorSet,													// dstSet
		1,																// dstBinding
		0,																// dstArrayElement
		1,																// descriptorCount
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,						// descriptorType
		nullptr,														// pImageInfo
		&modelBI,														// pBufferInfo
		nullptr,														// pTexelBufferView
	};

	std::vector<VkWriteDescriptorSet> writeDescriptorSets = { viewProjWriteDescriptorSet, modelWriteDescriptorSet };

	vkUpdateDescriptorSets(mLogicalDevice->GetLogicalDevice(), (uint32_t)writeDescriptorSets.size(), writeDescriptorSets.data(), 0, nullptr);
}

void DescriptorSetWrapper::WriteTextureDescriptorSet(ImageViewWrapper* imageView, SamplerWrapper* sampler) {
	VkDescriptorImageInfo imageInfo = {
		.sampler = sampler->GetSampler(),
		.imageView = imageView->GetImageView(),
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};

	VkWriteDescriptorSet descriptorWrite = {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.pNext = nullptr,
		.dstSet = mDescriptorSet,
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImageInfo = &imageInfo,
		.pBufferInfo = nullptr,
		.pTexelBufferView = nullptr
	};

	vkUpdateDescriptorSets(mLogicalDevice->GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
}

VkDescriptorSet DescriptorSetWrapper::GetDescriptorSet() {
	return mDescriptorSet;
}

void DescriptorSetWrapper::CreateGenericDescriptorSet() {
	VkDescriptorSetLayout layout = mDescriptorSetLayout->GetDescriptorSetLayout();
	VkDescriptorSetAllocateInfo descriptorSetAI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,					// sType
		nullptr,														// pNext
		mDescriptorPool->GetDescriptorPool(),							// descriptorPool
		1,																// descriptorSetCount
		&layout															// pSetLayouts
	};

	VkResult result = vkAllocateDescriptorSets(mLogicalDevice->GetLogicalDevice(), &descriptorSetAI, &mDescriptorSet);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Set allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Descriptor Set! Error Code: " + NT_CHECK_RESULT(result));
	}
}


/*

	Do not really need another function to create dynamic descriptor sets since they are basically the same. 
	But for pedagogical reasons I will keep it. 

*/
void DescriptorSetWrapper::CreateDynamicDescriptorSet() {
	VkDescriptorSetLayout layout = mDescriptorSetLayout->GetDescriptorSetLayout();
	VkDescriptorSetAllocateInfo descriptorSetAI = {
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,					// sType
		nullptr,														// pNext
		mDescriptorPool->GetDescriptorPool(),							// descriptorPool
		1,																// descriptorSetCount
		&layout															// pSetLayouts
	};

	VkResult result = vkAllocateDescriptorSets(mLogicalDevice->GetLogicalDevice(), &descriptorSetAI, &mDescriptorSet);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Set allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Descriptor Set! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void DescriptorSetWrapper::CreateTextureDescriptorSet() {
	VkDescriptorSetLayout layout = mDescriptorSetLayout->GetDescriptorSetLayout();
	VkDescriptorSetAllocateInfo descriptorSetAI = {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.pNext = nullptr,
		.descriptorPool = mDescriptorPool->GetDescriptorPool(),
		.descriptorSetCount = 1,
		.pSetLayouts = &layout
	};

	VkResult result = vkAllocateDescriptorSets(mLogicalDevice->GetLogicalDevice(), &descriptorSetAI, &mDescriptorSet);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Descriptor Set allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Descriptor Set! Error Code: " + NT_CHECK_RESULT(result));
	}
}
