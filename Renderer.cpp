#include "Renderer.h"
#include "globals.h"
#include "Context.h"
#include "LogicalDeviceWrapper.h"
#include "PhysicalDeviceWrapper.h"
#include "SwapchainWrapper.h"
#include "RenderPassWrapper.h"
#include "ShaderWrapper.h"
#include "PipelineWrapper.h"
#include "FramebufferWrapper.h"
#include "CommandPoolWrapper.h"
#include "CommandBufferWrapper.h"
#include "SynchronizationWrapper.h"
#include "BufferWrapper.h"
#include "Mesh.h"
#include "DescriptorWrapper.h"
#include "ImageWrapper.h"
#include "ImageViewWrapper.h"
#include "SamplerWrapper.h"

Renderer::Renderer() {
	mContext = new Context();
	mDescriptorSetLayout = new DescriptorSetLayoutWrapper(mContext, DYNAMIC);
	mTSDescriptorSetLayout = new DescriptorSetLayoutWrapper(mContext, TEXTURE);
	std::vector<DescriptorSetLayoutWrapper*> layouts = { mDescriptorSetLayout, mTSDescriptorSetLayout };
	mPipeline = new PipelineWrapper(mContext, layouts);
	mGraphicsCommandPool = new CommandPoolWrapper(mContext, mContext->mPhysicalDevice->GetQueueFamilyIndices().mGraphics);
	mTransferCommandPool = new CommandPoolWrapper(mContext, mContext->mPhysicalDevice->GetQueueFamilyIndices().mTransfer);
	mDescriptorPool = new DescriptorPoolWrapper(mContext, DYNAMIC);
	mTDescriptorPool = new DescriptorPoolWrapper(mContext, TEXTURE);
	mDepthImage = new ImageWrapper(mContext, mGraphicsCommandPool, WINDOW_WIDTH, WINDOW_HEIGHT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	mDepthImageView = new ImageViewWrapper(mContext, mDepthImage->GetImage(), VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_ASPECT_DEPTH_BIT);
	mTextureImage = new ImageWrapper(mContext, mGraphicsCommandPool, ".\\Resources\\Textures\\container2.png");
	mTextureImageView = new ImageViewWrapper(mContext, mTextureImage->GetImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	allocateDynamicBufferTransferSpace();	// Figure out the spacing necessary for aligned memory access of dynamic buffers
	for (size_t i = 0; i < mContext->mSwapchain->GetSwapchainImages().size(); i++) {
		mFramebuffers.push_back(new FramebufferWrapper(mContext, (int)i, mDepthImageView));
		mCommandBuffers.push_back(new CommandBufferWrapper(mContext, mGraphicsCommandPool));
		mUniformBuffers.push_back(new BufferWrapper(mContext, (VkDeviceSize)sizeof(mVP), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		mDynamicUniformBuffers.push_back(new BufferWrapper(mContext, (VkDeviceSize)(mModelUniformAlignment * MAX_OBJECTS), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		mDescriptorSets.push_back(new DescriptorSetWrapper(mContext, mDescriptorSetLayout, mDescriptorPool, DYNAMIC));
		mDescriptorSets.at(i)->WriteDynamicDescriptorSet(mUniformBuffers.at(i), mDynamicUniformBuffers.at(i));
	}

	for (size_t i = 0; i < MAX_FRAMES_DRAW; i++) {
		mImageAvailableSemaphores.push_back(new SemaphoreWrapper(mContext));
		mRenderFinishedSemaphores.push_back(new SemaphoreWrapper(mContext));
		mDrawFences.push_back(new FenceWrapper(mContext, VK_FENCE_CREATE_SIGNALED_BIT));
	}
	mSampler = new SamplerWrapper(mContext);

	mTextureDescriptorSet = new DescriptorSetWrapper(mContext, mTSDescriptorSetLayout, mTDescriptorPool, TEXTURE);
	mTextureDescriptorSet->WriteTextureDescriptorSet(mTextureImageView, mSampler);

	std::vector<Vertex> cubeVertices = {
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },	// 0 - A
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },	// 1 - B
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },	// 2 - C
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },	// 3 - D
		{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },	// 4 - E
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },	// 5 - F
		{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },	// 6 - G
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }	// 7 - H
	};
	std::vector<uint32_t> cubeIndices = {

		6, 3, 2,
		6, 7, 3,
		2, 1, 0,
		2, 3, 1,
		0, 1, 4,
		1, 5, 4,
		4, 5, 6,
		5, 7, 6,
		4, 6, 2,
		4, 2, 0,
		5, 3, 7,
		5, 1, 3


		/*
		
		4, 2, 0,
		2, 7, 3,
		6, 5, 7,
		1, 7, 5,
		0, 3, 1,
		4, 1, 5,
		4, 6, 2,
		2, 6, 7,
		6, 4, 5,
		1, 3, 7,
		0, 2, 3,
		4, 0, 1
		
		*/
	};

	std::vector<Vertex> texturedMeshVertices = {
		{ {-1.0f,  1.0f, 0.0}, {5.0, 0.0, 0.0}, { 1.0f, 1.0f }},	// 0
		{ {-1.0f, -1.0f, 0.0}, {5.0, 0.0, 0.0}, { 1.0f, 0.0f }},	// 1
		{ { 1.0f, -1.0f, 0.0}, {5.0, 0.0, 0.0}, { 0.0f, 0.0f }},	// 2
		{ { 1.0f,  1.0f, 0.0}, {5.0, 0.0, 0.0}, { 0.0f, 1.0f }},	// 3
	};

	std::vector<uint32_t> texturedMeshIndices = {
		0, 1, 2,
		2, 3, 0
	};

	mMeshList.push_back(new Mesh(mContext, mTransferCommandPool, &cubeVertices, &cubeIndices));
	mMeshList.push_back(new Mesh(mContext, mTransferCommandPool, &cubeVertices, &cubeIndices));
	mMeshList.push_back(new Mesh(mContext, mTransferCommandPool, &texturedMeshVertices, &texturedMeshIndices));

	RecordCommands();

	mVP.mProjection = glm::perspective(glm::radians(45.0f), (float)mContext->mSwapchain->GetSwapchainExtent().width / (float)mContext->mSwapchain->GetSwapchainExtent().height, 0.1f, 1000.0f);
	mVP.mView = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mVP.mProjection[1][1] *= -1;

	mCurrentFrame = 0;
}

Renderer::~Renderer() {
	vkDeviceWaitIdle(mContext->mLogicalDevice->GetLogicalDevice());

	_aligned_free(mModelTransferSpace);

	// Don't forget to insert in reverse order
	for (size_t i = 0; i < mMeshList.size(); i++) {
		delete mMeshList.at(i);
	}
	delete mSampler;
	for (size_t i = 0; i < MAX_FRAMES_DRAW; i++) {
		delete mDrawFences.at(i);
		delete mRenderFinishedSemaphores.at(i);
		delete mImageAvailableSemaphores.at(i);
	}
	for (size_t i = 0; i < mCommandBuffers.size(); i++) {
		delete mDescriptorSets.at(i);
		delete mDynamicUniformBuffers.at(i);
		delete mUniformBuffers.at(i);
		delete mCommandBuffers.at(i);
	}
	delete mTextureImageView;
	delete mTextureImage;
	delete mDepthImageView;
	delete mDepthImage;
	delete mTDescriptorPool;
	delete mDescriptorPool;
	delete mTransferCommandPool;
	delete mGraphicsCommandPool;
	for (size_t i = 0; i < mFramebuffers.size(); i++) {
		delete mFramebuffers.at(i);
	}
	delete mPipeline;
	delete mTSDescriptorSetLayout;
	delete mDescriptorSetLayout;
	delete mContext;
}

void Renderer::Draw() {
	VkFence drawFence = mDrawFences.at(mCurrentFrame)->GetFence();
	vkWaitForFences(mContext->mLogicalDevice->GetLogicalDevice(), 1, &drawFence, VK_TRUE, UINT64_MAX);
	vkResetFences(mContext->mLogicalDevice->GetLogicalDevice(), 1, &drawFence);

	/// Grab next available image
	uint32_t imageIndex;
	vkAcquireNextImageKHR(mContext->mLogicalDevice->GetLogicalDevice(), mContext->mSwapchain->GetSwapchain(), UINT64_MAX, mImageAvailableSemaphores.at(mCurrentFrame)->GetSemaphore(), VK_NULL_HANDLE, &imageIndex);

	// Update Uniform Buffers and Dynamic Uniform Buffers
	mUniformBuffers.at(imageIndex)->MapBufferMemory(&mVP, sizeof(mVP));

	// Update mModelTransferSpace with all of the fresh model information
	for (size_t i = 0; i < mMeshList.size(); i++) {
		glm::mat4* thisModel = (glm::mat4*)((uint64_t)mModelTransferSpace + (mModelUniformAlignment * i));
		*thisModel = mMeshList.at(i)->GetModel();
	}

	// Push newly updated model info to graphics card
	mDynamicUniformBuffers.at(imageIndex)->MapBufferMemory(mModelTransferSpace, mModelUniformAlignment * mMeshList.size());

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSemaphore waitSemaphore = mImageAvailableSemaphores.at(mCurrentFrame)->GetSemaphore();
	VkCommandBuffer commandBuffer = mCommandBuffers.at(imageIndex)->GetCommandBuffer();
	VkSemaphore signalSemaphore = mRenderFinishedSemaphores.at(mCurrentFrame)->GetSemaphore();

	VkSubmitInfo queueSI = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &waitSemaphore,
		.pWaitDstStageMask = waitStages,
		.commandBufferCount = 1,
		.pCommandBuffers = &commandBuffer,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &signalSemaphore
	};

	VkResult result = vkQueueSubmit(mContext->mLogicalDevice->GetGraphicsQueue(), 1, &queueSI, mDrawFences.at(mCurrentFrame)->GetFence());
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer to queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkSwapchainKHR swapchain= mContext->mSwapchain->GetSwapchain();
	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &signalSemaphore,
		.swapchainCount = 1,
		.pSwapchains = &swapchain,
		.pImageIndices = &imageIndex
	};

	result = vkQueuePresentKHR(mContext->mLogicalDevice->GetPresentQueue(), &presentInfo);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present Image! Error Code: " + NT_CHECK_RESULT(result));
	}

	mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_DRAW;
}

void Renderer::UpdateModel(int modelID, glm::mat4 model) {
	if (modelID < 0 || modelID > mMeshList.size())
		throw std::runtime_error("Attempt to access model index out of range!");
	mMeshList.at(modelID)->SetModel(model);
}

void Renderer::UpdateCamera(glm::mat4 view) {
	mVP.mView = view;
}

Context* Renderer::GetContext() {
	return mContext;
}

void Renderer::RecordCommands() {
	VkCommandBufferBeginInfo commandBufferBI = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		.pInheritanceInfo = nullptr
	};
	VkClearValue clearValue = { 0.0f, 0.0f, 0.2f, 1.0f };
	VkClearValue depthValue = { }; depthValue.depthStencil.depth = 1.0f;
	std::vector<VkClearValue> clearValues = { clearValue, depthValue };
	VkRenderPassBeginInfo renderPassBI = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.pNext = nullptr,
		.renderPass = mContext->mRenderPass->GetRenderPass(),
		.framebuffer = VK_NULL_HANDLE,
		.renderArea = {
			.offset = {
				.x = 0,
				.y = 0
			},
			.extent = {
				.width = mContext->mSwapchain->GetSwapchainExtent().width,
				.height = mContext->mSwapchain->GetSwapchainExtent().height
			}
		},
		.clearValueCount = (uint32_t)clearValues.size(),
		.pClearValues = clearValues.data()
	};

	for (size_t i = 0; i < mCommandBuffers.size(); i++) {
		renderPassBI.framebuffer = mFramebuffers.at(i)->GetFramebuffer();

		VkResult result;
		result = vkBeginCommandBuffer(mCommandBuffers.at(i)->GetCommandBuffer(), &commandBufferBI);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording command buffer! Error Code: " + NT_CHECK_RESULT(result));
		}

			vkCmdBeginRenderPass(mCommandBuffers.at(i)->GetCommandBuffer(), &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(mCommandBuffers.at(i)->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline->GetPipeline());

				for (size_t j = 0; j < mMeshList.size(); j++) {
					VkBuffer vertexBuffers[] = { mMeshList.at(j)->GetVertexBuffer()->GetBuffer() };
					VkDeviceSize offsets[] = { 0 };

					vkCmdBindVertexBuffers(mCommandBuffers.at(i)->GetCommandBuffer(), 0, 1, vertexBuffers, offsets);

					vkCmdBindIndexBuffer(mCommandBuffers.at(i)->GetCommandBuffer(), mMeshList.at(j)->GetIndexBuffer()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

					uint32_t dynamicOffset = (uint32_t)(mModelUniformAlignment * j);

					std::vector<VkDescriptorSet> descriptorSets = { mDescriptorSets.at(i)->GetDescriptorSet(), mTextureDescriptorSet->GetDescriptorSet()};
						
					vkCmdBindDescriptorSets(mCommandBuffers.at(i)->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline->GetPipelineLayout(), 0, (uint32_t)descriptorSets.size(), descriptorSets.data(), 1, &dynamicOffset);
				
					vkCmdDrawIndexed(mCommandBuffers.at(i)->GetCommandBuffer(), mMeshList.at(j)->GetIndexCount(), 1, 0, 0, 0);
				}

			vkCmdEndRenderPass(mCommandBuffers.at(i)->GetCommandBuffer());

		result = vkEndCommandBuffer(mCommandBuffers.at(i)->GetCommandBuffer());
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to end recording command buffer! Error Code: " + NT_CHECK_RESULT(result));
		}
	}
}

void Renderer::allocateDynamicBufferTransferSpace() {
	// Grab the minimum required size for uniform buffer offsets
	mMinUniformBufferOffset = mContext->mPhysicalDevice->GetPhysicalDeviceProperties().limits.minUniformBufferOffsetAlignment;

	// Do some weird bit shifting magic
	mModelUniformAlignment = (sizeof(glm::mat4) + mMinUniformBufferOffset - 1) & ~(mMinUniformBufferOffset - 1);

	// Create space in memory to hold the dynamic buffer that is now aligned and holds MAX_OBJECTS
	mModelTransferSpace = (glm::mat4*)_aligned_malloc(mModelUniformAlignment * MAX_OBJECTS, mModelUniformAlignment);
}
