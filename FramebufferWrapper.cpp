#include "FramebufferWrapper.h"
#include "globals.h"
#include "Context.h"
#include "LogicalDeviceWrapper.h"
#include "SwapchainWrapper.h"
#include "ImageViewWrapper.h"
#include "RenderPassWrapper.h"

FramebufferWrapper::FramebufferWrapper(Context* context, int index) : mContext(context) {
	CreateFramebuffer(index);
}

FramebufferWrapper::FramebufferWrapper(Context* context, int index, ImageViewWrapper* imageView) : mContext(context) {
	CreateFramebuffer(index, imageView);
}


FramebufferWrapper::~FramebufferWrapper() {
	vkDestroyFramebuffer(mContext->mLogicalDevice->GetLogicalDevice(), mFramebuffer, nullptr); std::cout << "Success: Framebuffer destroyed." << std::endl;
}

VkFramebuffer FramebufferWrapper::GetFramebuffer() {
	return mFramebuffer;
}

void FramebufferWrapper::CreateFramebuffer(int index) {
	std::vector<VkImageView> framebufferAttachments = {
		mContext->mSwapchain->GetSwapchainImages().at(index).mImageView->GetImageView()
	};
	VkFramebufferCreateInfo framebufferCI = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,						// sType
		nullptr,														// pNext
		0,																// flags
		mContext->mRenderPass->GetRenderPass(),							// renderPass
		static_cast<uint32_t>(framebufferAttachments.size()),			// attachmentCount
		framebufferAttachments.data(),									// pAttachments
		mContext->mSwapchain->GetSwapchainExtent().width,				// width
		mContext->mSwapchain->GetSwapchainExtent().height,				// height
		1																// layers
	};

	VkResult result = vkCreateFramebuffer(mContext->mLogicalDevice->GetLogicalDevice(), &framebufferCI, nullptr, &mFramebuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Framebuffer created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create framebuffer! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void FramebufferWrapper::CreateFramebuffer(int index, ImageViewWrapper* imageView) {
	std::vector<VkImageView> framebufferAttachments = {
		mContext->mSwapchain->GetSwapchainImages().at(index).mImageView->GetImageView(),
		imageView->GetImageView()
	};
	VkFramebufferCreateInfo framebufferCI = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,						// sType
		nullptr,														// pNext
		0,																// flags
		mContext->mRenderPass->GetRenderPass(),							// renderPass
		static_cast<uint32_t>(framebufferAttachments.size()),			// attachmentCount
		framebufferAttachments.data(),									// pAttachments
		mContext->mSwapchain->GetSwapchainExtent().width,				// width
		mContext->mSwapchain->GetSwapchainExtent().height,				// height
		1																// layers
	};

	VkResult result = vkCreateFramebuffer(mContext->mLogicalDevice->GetLogicalDevice(), &framebufferCI, nullptr, &mFramebuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Framebuffer created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create framebuffer! Error Code: " + NT_CHECK_RESULT(result));
	}
}
