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

Renderer::Renderer(WindowWrapper* window) : mWindow(window) {
	mInstance = new InstanceWrapper();
	mSurface = new SurfaceWrapper(mWindow, mInstance);
	mPhysicalDevice = new PhysicalDeviceWrapper(mInstance, mSurface);
	mLogicalDevice = new LogicalDeviceWrapper(mPhysicalDevice);
	mSwapchain = new SwapchainWrapper(mPhysicalDevice, mLogicalDevice, mSurface);
	mRenderPass = new RenderPassWrapper(mLogicalDevice, mSurface);
	mPipeline = new PipelineWrapper(mLogicalDevice, mRenderPass);
	for (size_t i = 0; i < mSwapchain->GetSwapchainImages().size(); i++) {
		mFramebuffers.push_back(new FramebufferWrapper(mLogicalDevice, mSwapchain, mRenderPass, (int)i));
	}
	mGraphicsCommandPool = new CommandPoolWrapper(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mGraphics);
	mTransferCommandPool = new CommandPoolWrapper(mLogicalDevice, mPhysicalDevice->GetQueueFamilyIndices().mTransfer);
	for (size_t i = 0; i < mSwapchain->GetSwapchainImages().size(); i++) {
		mCommandBuffers.push_back(new CommandBufferWrapper(mLogicalDevice, mGraphicsCommandPool));
	}
	for (size_t i = 0; i < MAX_FRAMES_DRAW; i++) {
		mImageAvailableSemaphores.push_back(new SemaphoreWrapper(mLogicalDevice));
		mRenderFinishedSemaphores.push_back(new SemaphoreWrapper(mLogicalDevice));
		mDrawFences.push_back(new FenceWrapper(mLogicalDevice, VK_FENCE_CREATE_SIGNALED_BIT));
	}

	std::vector<Vertex> meshVertices = {
		{ {  0.5f,  0.5f, 0.0f }, {1.0f, 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f }, {0.0f, 1.0f, 0.0f } },
		{ { -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, 0.0f }, {1.0f, 1.0f, 0.0f } },
	};

	std::vector<uint32_t> meshIndices = {
		0, 1, 3,
		1, 2, 3
	};

	mFirstMesh = new Mesh(mPhysicalDevice, mLogicalDevice, mTransferCommandPool, &meshVertices, &meshIndices);

	RecordCommands();


	mCurrentFrame = 0;
}

Renderer::~Renderer() {
	vkDeviceWaitIdle(mLogicalDevice->GetLogicalDevice());

	// Don't forget to insert in reverse order
	delete mFirstMesh;
	for (size_t i = 0; i < MAX_FRAMES_DRAW; i++) {
		delete mDrawFences.at(i);
		delete mRenderFinishedSemaphores.at(i);
		delete mImageAvailableSemaphores.at(i);
	}
	for (size_t i = 0; i < mCommandBuffers.size(); i++) {
		delete mCommandBuffers.at(i);
	}
	delete mTransferCommandPool;
	delete mGraphicsCommandPool;
	for (size_t i = 0; i < mFramebuffers.size(); i++) {
		delete mFramebuffers.at(i);
	}
	delete mPipeline;
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

	uint32_t imageIndex;
	vkAcquireNextImageKHR(mLogicalDevice->GetLogicalDevice(), mSwapchain->GetSwapchain(), UINT64_MAX, mImageAvailableSemaphores.at(mCurrentFrame)->GetSemaphore(), VK_NULL_HANDLE, &imageIndex);


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

void Renderer::RecordCommands() {
	VkCommandBufferBeginInfo commandBufferBI = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,						// sType
		nullptr,															// pNext
		VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,						// flags
		nullptr																// pInheritanceInfo
	};
	VkClearValue clearValue = { 0.0f, 0.0f, 0.2f, 1.0f };
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
		1,																	// clearValueCount
		&clearValue															// pClearValues
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

				VkBuffer vertexBuffers[] = { mFirstMesh->GetVertexBuffer()->GetBuffer() };

				VkDeviceSize offsets[] = { 0 };

				vkCmdBindVertexBuffers(mCommandBuffers.at(i)->GetCommandBuffer(), 0, 1, vertexBuffers, offsets);

				vkCmdBindIndexBuffer(mCommandBuffers.at(i)->GetCommandBuffer(), mFirstMesh->GetIndexBuffer()->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

				//vkCmdDraw(mCommandBuffers.at(i)->GetCommandBuffer(), (uint32_t)mFirstMesh->GetVertexCount(), 1, 0, 0);
				vkCmdDrawIndexed(mCommandBuffers.at(i)->GetCommandBuffer(), (uint32_t)mFirstMesh->GetIndexCount(), 1, 0, 0, 0);

			vkCmdEndRenderPass(mCommandBuffers.at(i)->GetCommandBuffer());

		result = vkEndCommandBuffer(mCommandBuffers.at(i)->GetCommandBuffer());
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to end recording command buffer! Error Code: " + NT_CHECK_RESULT(result));
		}
	}
}