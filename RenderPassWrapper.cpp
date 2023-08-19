#include "RenderPassWrapper.h"
#include "globals.h"
#include "SurfaceWrapper.h"
#include "LogicalDeviceWrapper.h"

RenderPassWrapper::RenderPassWrapper(LogicalDeviceWrapper* lDevice, SurfaceWrapper* surface) : mLogicalDevice(lDevice), mSurface(surface) {
	CreateDepthRenderPass();
}

RenderPassWrapper::~RenderPassWrapper() {
	vkDestroyRenderPass(mLogicalDevice->GetLogicalDevice(), mRenderPass, nullptr); std::cout << "Success: Render Pass destroyed." << std::endl;
}

VkRenderPass RenderPassWrapper::GetRenderPass() {
	return mRenderPass;
}

void RenderPassWrapper::CreateGenericRenderPass() {
	// Attachment Descriptions
	// - Color Attachment
	VkAttachmentDescription colorAttachment {
		0,																			// flags
		mSurface->GetBestSurfaceFormat().format,									// format
		VK_SAMPLE_COUNT_1_BIT,														// samples
		VK_ATTACHMENT_LOAD_OP_CLEAR,												// loadOp
		VK_ATTACHMENT_STORE_OP_STORE,												// storeOp
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,											// stencilLoadOp
		VK_ATTACHMENT_STORE_OP_DONT_CARE,											// stencilStoreOp
		VK_IMAGE_LAYOUT_UNDEFINED,													// initialLayout
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR												// finalLayout
	};

	// Put Attachments together in a vector
	std::vector<VkAttachmentDescription> attachmentDescriptions = { colorAttachment };

	// Attachment References
	// - Color Attachment Reference
	VkAttachmentReference colorAttachmentRef {
		0,																			// attachment
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL									// layout
	};

	// Subpass Descriptions
	// - First Subpass
	VkSubpassDescription firstSubpass {
		0,																			// flags
		VK_PIPELINE_BIND_POINT_GRAPHICS,											// pipelineBindPoint
		0,																			// inputAttachmentCount
		nullptr,																	// pInputAttachments
		1,																			// colorAttachmentCount
		&colorAttachmentRef,														// pColorAttachments
		nullptr,																	// pResolveAttachments
		nullptr,																	// pDepthStencilAttachment
		0,																			// preserveAttachmentCount
		nullptr																		// pPreserveAttachments
	};

	// Put Subpasses together in a vector
	std::vector<VkSubpassDescription> subpasses = { firstSubpass };

	// Subpass Dependencies
	// - - Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	VkSubpassDependency firstSubpassDependency {
		VK_SUBPASS_EXTERNAL,														// srcSubpass
		0,																			// dstSubpass
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,										// srcStageMask
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// dstStageMask
		VK_ACCESS_MEMORY_READ_BIT,													// srcAccessMask
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// dstAccessMask
		0																			// dependencyFlags
	};
	VkSubpassDependency secondSubpassDependency {
		0,																			// srcSubpass
		VK_SUBPASS_EXTERNAL,														// dstSubpass
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// srcStageMask
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,										// dstStageMask
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// srcAccessMask
		VK_ACCESS_MEMORY_READ_BIT,													// dstAccessMask
		0																			// dependencyFlags
	};

	// Put Subpass Dependencies in a vector
	std::vector<VkSubpassDependency> subpassDependencies = { firstSubpassDependency, secondSubpassDependency };

	// Render Pass create info structure
	VkRenderPassCreateInfo renderPassCI = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,									// sType
		nullptr,																	// pNext
		0,																			// flags
		(uint32_t)attachmentDescriptions.size(),									// attachmentCount
		attachmentDescriptions.data(),												// pAttachments
		(uint32_t)subpasses.size(),													// subpassCount
		subpasses.data(),															// pSubpasses
		(uint32_t)subpassDependencies.size(),										// dependencyCount
		subpassDependencies.data()													// pDependencies
	};

	// Create Render Pass
	VkResult result = vkCreateRenderPass(mLogicalDevice->GetLogicalDevice(), &renderPassCI, nullptr, &mRenderPass);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Render Pass created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Render Pass! Error Code: " + NT_CHECK_RESULT(result));
	}
}

void RenderPassWrapper::CreateDepthRenderPass() {
	// Attachment Descriptions
	// - Color Attachment
	VkAttachmentDescription colorAttachment {
		0,																			// flags
		mSurface->GetBestSurfaceFormat().format,									// format
		VK_SAMPLE_COUNT_1_BIT,														// samples
		VK_ATTACHMENT_LOAD_OP_CLEAR,												// loadOp
		VK_ATTACHMENT_STORE_OP_STORE,												// storeOp
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,											// stencilLoadOp
		VK_ATTACHMENT_STORE_OP_DONT_CARE,											// stencilStoreOp
		VK_IMAGE_LAYOUT_UNDEFINED,													// initialLayout
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR												// finalLayout
	};

	VkAttachmentDescription depthAttachment{
		0,																			// flags
		VK_FORMAT_D32_SFLOAT_S8_UINT,												// format
		VK_SAMPLE_COUNT_1_BIT,														// samples
		VK_ATTACHMENT_LOAD_OP_CLEAR,												// loadOp
		VK_ATTACHMENT_STORE_OP_DONT_CARE,											// storeOp
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,											// stencilLoadOp
		VK_ATTACHMENT_STORE_OP_DONT_CARE,											// stencilstoreOp
		VK_IMAGE_LAYOUT_UNDEFINED,													// initialLayout
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL							// finalLayout
	};

	// Put Attachments together in a vector
	std::vector<VkAttachmentDescription> attachmentDescriptions = { colorAttachment, depthAttachment };

	// Attachment References
	// - Color Attachment Reference
	VkAttachmentReference colorAttachmentRef {
		0,																			// attachment
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL									// layout
	};

	VkAttachmentReference depthAttachmentRef{
		1,																			// attachment
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL							// layout
	};

	// Subpass Descriptions
	// - First Subpass
	VkSubpassDescription firstSubpass{
		0,																			// flags
		VK_PIPELINE_BIND_POINT_GRAPHICS,											// pipelineBindPoint
		0,																			// inputAttachmentCount
		nullptr,																	// pInputAttachments
		1,																			// colorAttachmentCount
		&colorAttachmentRef,														// pColorAttachments
		nullptr,																	// pResolveAttachments
		&depthAttachmentRef,														// pDepthStencilAttachment
		0,																			// preserveAttachmentCount
		nullptr																		// pPreserveAttachments
	};

	// Put Subpasses together in a vector
	std::vector<VkSubpassDescription> subpasses = { firstSubpass };

	// Subpass Dependencies
	// - - Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	VkSubpassDependency firstSubpassDependency{
		VK_SUBPASS_EXTERNAL,														// srcSubpass
		0,																			// dstSubpass
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,										// srcStageMask
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// dstStageMask
		VK_ACCESS_MEMORY_READ_BIT,													// srcAccessMask
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// dstAccessMask
		0																			// dependencyFlags
	};
	VkSubpassDependency secondSubpassDependency{
		0,																			// srcSubpass
		VK_SUBPASS_EXTERNAL,														// dstSubpass
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,								// srcStageMask
		VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,										// dstStageMask
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,	// srcAccessMask
		VK_ACCESS_MEMORY_READ_BIT,													// dstAccessMask
		0																			// dependencyFlags
	};

	// Put Subpass Dependencies in a vector
	std::vector<VkSubpassDependency> subpassDependencies = { firstSubpassDependency, secondSubpassDependency };

	// Render Pass create info structure
	VkRenderPassCreateInfo renderPassCI = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,									// sType
		nullptr,																	// pNext
		0,																			// flags
		(uint32_t)attachmentDescriptions.size(),									// attachmentCount
		attachmentDescriptions.data(),												// pAttachments
		(uint32_t)subpasses.size(),													// subpassCount
		subpasses.data(),															// pSubpasses
		(uint32_t)subpassDependencies.size(),										// dependencyCount
		subpassDependencies.data()													// pDependencies
	};

	// Create Render Pass
	VkResult result = vkCreateRenderPass(mLogicalDevice->GetLogicalDevice(), &renderPassCI, nullptr, &mRenderPass);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Render Pass created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Render Pass! Error Code: " + NT_CHECK_RESULT(result));
	}
}
