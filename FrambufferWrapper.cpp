#include "FrambufferWrapper.h"
#include "globals.h"
#include "LogicalDeviceWrapper.h"
#include "SwapchainWrapper.h"
#include "ImageViewWrapper.h"
#include "RenderPassWrapper.h"

FramebufferWrapper::FramebufferWrapper(LogicalDeviceWrapper* lDevice, SwapchainWrapper* swapchain, RenderPassWrapper* renderpass, int index) : mLogicalDevice(lDevice), mSwapchain(swapchain), mRenderPass(renderpass) {
	CreateFramebuffer(index);
}

FramebufferWrapper::~FramebufferWrapper() {
	vkDestroyFramebuffer(mLogicalDevice->GetLogicalDevice(), mFramebuffer, nullptr);
}

VkFramebuffer FramebufferWrapper::GetFramebuffer() {
	return mFramebuffer;
}

void FramebufferWrapper::CreateFramebuffer(int index) {
	std::vector<VkImageView> framebufferAttachments = {
		mSwapchain->GetSwapchainImages().at(index).mImageView->GetImageView()
	};
	VkFramebufferCreateInfo framebufferCI = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,						// sType
		nullptr,														// pNext
		0,																// flags
		mRenderPass->GetRenderPass(),									// renderPass
		static_cast<uint32_t>(framebufferAttachments.size()),			// attachmentCount
		framebufferAttachments.data(),									// pAttachments
		mSwapchain->GetSwapchainExtent().width,							// width
		mSwapchain->GetSwapchainExtent().height,						// height
		1																// layers
	};

	VkResult result = vkCreateFramebuffer(mLogicalDevice->GetLogicalDevice(), &framebufferCI, nullptr, &mFramebuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Framebuffer created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create framebuffer! Error Code: " + NT_CHECK_RESULT(result));
	}
}
