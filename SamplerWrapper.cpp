#include "SamplerWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"

SamplerWrapper::SamplerWrapper(LogicalDeviceWrapper* lDevice) : mLogicalDevice(lDevice) {
	CreateSampler();
}

SamplerWrapper::~SamplerWrapper() {
	vkDestroySampler(mLogicalDevice->GetLogicalDevice(), mSampler, nullptr); std::cout << "Success: Sampler Destroyed." << std::endl;
}

VkSampler SamplerWrapper::GetSampler() {
	return mSampler;
}

void SamplerWrapper::CreateSampler() {
	VkSamplerCreateInfo samplerCI = {
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.mipLodBias = 0.0f,
		.anisotropyEnable = VK_TRUE,
		.maxAnisotropy = 16,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_NEVER,
		.minLod = 0.0f,
		.maxLod = 0.0f,
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE
	};

	VkResult result = vkCreateSampler(mLogicalDevice->GetLogicalDevice(), &samplerCI, nullptr, &mSampler);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Texture Sampler created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Texture Sampler! Error Code: " + NT_CHECK_RESULT(result));
	}
}