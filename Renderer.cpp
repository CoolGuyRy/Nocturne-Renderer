#include "Renderer.h"
#include "globals.h"
#include "WindowWrapper.h"
#include "InstanceWrapper.h"
#include "SurfaceWrapper.h"
#include "PhysicalDeviceWrapper.h"
#include "LogicalDeviceWrapper.h"
#include "SwapchainWrapper.h"
#include "ShaderWrapper.h"
#include "RenderPassWrapper.h"
#include "PipelineWrapper.h"
#include "FramebufferWrapper.h"
#include "CommandPoolWrapper.h"
#include "CommandBufferWrapper.h"
#include "SynchronizationWrapper.h"
#include "BufferWrapper.h"
#include "Mesh.h"
#include "DescriptorSetWrapper.h"
#include "ImageWrapper.h"
#include "ImageViewWrapper.h"

Renderer::Renderer(WindowWrapper* window) : mWindow(window) {
	mInstance = new InstanceWrapper();
	mSurface = new SurfaceWrapper(mWindow, mInstance);
	mPhysicalDevice = new PhysicalDeviceWrapper(mInstance, mSurface);
	mLogicalDevice = new LogicalDeviceWrapper(mPhysicalDevice);
	mSwapchain = new SwapchainWrapper(mPhysicalDevice, mLogicalDevice, mSurface);
	mRenderPass = new RenderPassWrapper(mLogicalDevice, mSurface);
	mDescriptorSetLayout = new DescriptorSetLayoutWrapper(mLogicalDevice);
	mPipeline = new PipelineWrapper(mLogicalDevice, mRenderPass, mDescriptorSetLayout);
	mGraphicsCommandPool = new CommandPoolWrapper(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mGraphics);
	mTransferCommandPool = new CommandPoolWrapper(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mTransfer);
	mDescriptorPool = new DescriptorPoolWrapper(mLogicalDevice);
	mDepthImage = new ImageWrapper(mPhysicalDevice, mLogicalDevice, WINDOW_WIDTH, WINDOW_HEIGHT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	mDepthImageView = new ImageViewWrapper(mLogicalDevice, mDepthImage->GetImage(), VK_FORMAT_D32_SFLOAT_S8_UINT, VK_IMAGE_ASPECT_DEPTH_BIT);
	allocateDynamicBufferTransferSpace();	// Figure out the spacing necessary for aligned memory access of dynamic buffers
	for (size_t i = 0; i < mSwapchain->GetSwapchainImages().size(); i++) {
		mFramebuffers.push_back(new FramebufferWrapper(mLogicalDevice, mSwapchain, mRenderPass, (int)i, mDepthImageView));
		mCommandBuffers.push_back(new CommandBufferWrapper(mLogicalDevice, mGraphicsCommandPool));
		mUniformBuffers.push_back(new BufferWrapper(mPhysicalDevice, mLogicalDevice, (VkDeviceSize)sizeof(mVP), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		mDynamicUniformBuffers.push_back(new BufferWrapper(mPhysicalDevice, mLogicalDevice, (VkDeviceSize)(mModelUniformAlignment * MAX_OBJECTS), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		mDescriptorSets.push_back(new DescriptorSetWrapper(mLogicalDevice, mDescriptorSetLayout, mDescriptorPool));
		mDescriptorSets.at(i)->WriteDynamicDescriptorSet(mUniformBuffers.at(i), mDynamicUniformBuffers.at(i));
	}
	for (size_t i = 0; i < MAX_FRAMES_DRAW; i++) {
		mImageAvailableSemaphores.push_back(new SemaphoreWrapper(mLogicalDevice));
		mRenderFinishedSemaphores.push_back(new SemaphoreWrapper(mLogicalDevice));
		mDrawFences.push_back(new FenceWrapper(mLogicalDevice, VK_FENCE_CREATE_SIGNALED_BIT));
	}

	std::vector<Vertex> cubeVertices = {
		{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f, 1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f } }
	};
	std::vector<uint32_t> cubeIndices = {
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
	};

	mMeshList.push_back(new Mesh(mPhysicalDevice, mLogicalDevice, mTransferCommandPool, &cubeVertices, &cubeIndices));
	mMeshList.push_back(new Mesh(mPhysicalDevice, mLogicalDevice, mTransferCommandPool, &cubeVertices, &cubeIndices));

	RecordCommands();

	mVP.mProjection = glm::perspective(glm::radians(45.0f), (float)mSwapchain->GetSwapchainExtent().width / (float)mSwapchain->GetSwapchainExtent().height, 0.1f, 1000.0f);
	mVP.mView = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mVP.mProjection[1][1] *= -1;

	mCurrentFrame = 0;
}

Renderer::~Renderer() {
	vkDeviceWaitIdle(mLogicalDevice->GetLogicalDevice());

	_aligned_free(mModelTransferSpace);

	// Don't forget to insert in reverse order
	for (size_t i = 0; i < mMeshList.size(); i++) {
		delete mMeshList.at(i);
	}
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
	delete mDepthImageView;
	delete mDepthImage;
	delete mDescriptorPool;
	delete mTransferCommandPool;
	delete mGraphicsCommandPool;
	for (size_t i = 0; i < mFramebuffers.size(); i++) {
		delete mFramebuffers.at(i);
	}
	delete mPipeline;
	delete mDescriptorSetLayout;
	delete mRenderPass;
	delete mSwapchain;
	delete mLogicalDevice;
	delete mPhysicalDevice;
	delete mSurface;
	delete mInstance;
}

void Renderer::Draw() {
	VkFence drawFence = mDrawFences.at(mCurrentFrame)->GetFence();
	vkWaitForFences(mLogicalDevice->GetLogicalDevice(), 1, &drawFence, VK_TRUE, UINT64_MAX);
	vkResetFences(mLogicalDevice->GetLogicalDevice(), 1, &drawFence);

	/// Grab next available image
	uint32_t imageIndex;
	vkAcquireNextImageKHR(mLogicalDevice->GetLogicalDevice(), mSwapchain->GetSwapchain(), UINT64_MAX, mImageAvailableSemaphores.at(mCurrentFrame)->GetSemaphore(), VK_NULL_HANDLE, &imageIndex);

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
		VK_STRUCTURE_TYPE_SUBMIT_INFO,										// sType
		nullptr,															// pNext
		1,																	// waitSemaphoreCount
		&waitSemaphore,														// pWaitSemaphores
		waitStages,															// pWaitDstStageMask
		1,																	// commandBufferCount
		&commandBuffer,														// pCommandBuffers
		1,																	// signalSemaphoreCount
		&signalSemaphore													// pSignalSemaphores
	};

	VkResult result = vkQueueSubmit(mLogicalDevice->GetGraphicsQueue(), 1, &queueSI, mDrawFences.at(mCurrentFrame)->GetFence());
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer to queue! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkSwapchainKHR swapchain= mSwapchain->GetSwapchain();
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,									// sType
		nullptr,															// pNext
		1,																	// waitSemaphoreCount
		&signalSemaphore,													// pWaitSemaphores
		1,																	// swapchainCount
		&swapchain,															// pSwapchains
		&imageIndex															// pImageIndices
	};

	result = vkQueuePresentKHR(mLogicalDevice->GetPresentQueue(), &presentInfo);
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

void Renderer::RecordCommands() {
	VkCommandBufferBeginInfo commandBufferBI = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,						// sType
		nullptr,															// pNext
		VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,						// flags
		nullptr																// pInheritanceInfo
	};
	VkClearValue clearValue = { 0.0f, 0.0f, 0.2f, 1.0f };
	VkClearValue depthValue; depthValue.depthStencil.depth = 1.0f;
	std::vector<VkClearValue> clearValues = { clearValue, depthValue };
	VkRenderPassBeginInfo renderPassBI = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,							// sType
		nullptr,															// pNext
		mRenderPass->GetRenderPass(),										// renderPass
		VK_NULL_HANDLE,														// framebuffer
		{																	// renderArea
			{																// offset
				0,															// x
				0															// y
			},
			{																// extent
				mSwapchain->GetSwapchainExtent().width,						// width
				mSwapchain->GetSwapchainExtent().height						// height
			}
		},
		clearValues.size(),													// clearValueCount
		clearValues.data()													// pClearValues
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

					VkDescriptorSet descriptorSet = mDescriptorSets.at(i)->GetDescriptorSet();
						
					vkCmdBindDescriptorSets(mCommandBuffers.at(i)->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline->GetPipelineLayout(), 0, 1, &descriptorSet, 1, &dynamicOffset);
				
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
	mMinUniformBufferOffset = mPhysicalDevice->GetPhysicalDeviceProperties().limits.minUniformBufferOffsetAlignment;

	// Do some weird bit shifting magic
	mModelUniformAlignment = (sizeof(glm::mat4) + mMinUniformBufferOffset - 1) & ~(mMinUniformBufferOffset - 1);

	// Create space in memory to hold the dynamic buffer that is now aligned and holds MAX_OBJECTS
	mModelTransferSpace = (glm::mat4*)_aligned_malloc(mModelUniformAlignment * MAX_OBJECTS, mModelUniformAlignment);
}
