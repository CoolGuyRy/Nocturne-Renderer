#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

#include "ImageView.h"
#include "Shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void Run();
private:
	// Run() Support Functions
	void Input();
	void Update(double);
	void Render();

	// Window Members / Functions
	GLFWwindow* mWindow;
	void CreateGLFWWindow();
	void DestroyGLFWWindow();

	// Instance Members / Functions
	VkInstance mInstance;
	std::vector<const char*> mExtensions;
	void CreateInstance();
	void DestroyInstance();
	bool CheckInstanceExtensionSupport();
	bool CheckValidationLayerSupport();

	// Surface Members / Functions
	VkSurfaceKHR mSurface;
	VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
	VkSurfaceFormatKHR mBestSurfaceFormat;
	VkPresentModeKHR mBestPresentMode;
	std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;
	std::vector<VkPresentModeKHR> mAvailablePresentModes;
	void CreateSurface();
	void DestroySurface();
	void AcquireSurfaceProperties(VkPhysicalDevice);

	// Physical Device Members / Functions
	VkPhysicalDevice mPhysicalDevice;
	VkPhysicalDeviceProperties mPhysicalDeviceProperties;
	VkPhysicalDeviceFeatures mPhysicalDeviceFeatures;
	VkPhysicalDeviceMemoryProperties mPhysicalDeviceMemoryProperties;
	struct QueueFamilyIndices {
		int mGraphics = -1;
		int mPresent = -1;
		int mCompute = -1;
		int mTransfer = -1;
		int mSparseBinding = -1;
	} mQueueFamilyIndices;
	void RetrievePhysicalDevice();
	bool CheckDeviceSuitable(VkPhysicalDevice);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice);
	void AssignQueueFamilyIndices();
	void ValidateQueueFamilyIndices();

	// Logical Device Members / Functions
	VkDevice mLogicalDevice;
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;
	VkQueue mTransferQueue;
	void CreateLogicalDevice();
	void DestroyLogicalDevice();

	// Swapchain Members / Functions
	VkSwapchainKHR mSwapchain;
	VkExtent2D mSwapchainExtent;
	struct SwapchainImage {
		VkImage mImage;
		ImageView* mImageView;
	};
	std::vector<SwapchainImage> mSwapchainImages;
	void CreateSwapchain();
	void DestroySwapchain();

	// Render Pass Members / Functions
	VkRenderPass mRenderPass;
	void CreateRenderPass();
	void DestroyRenderPass();

	// Graphics Pipeline Members / Functions
	VkPipeline mPipeline;
	VkPipelineLayout mPipelineLayout;
	void CreateGraphicsPipeline();
	void DestroyGraphicsPipeline();

	// Framebuffer Memebers / Functions
	std::vector<VkFramebuffer> mFramebuffers;
	void CreateFramebuffers();
	void DestroyFrameBuffers();

	// Command Pool Members / Functions
	VkCommandPool mGraphicsCommandPool;
	VkCommandPool mTransferCommandPool;
	void CreateCommandPool(VkCommandPool&, uint32_t);
	void DestroyCommandPool(VkCommandPool);

	// Command Buffer Members / Functions
	VkCommandBuffer mCommandBuffer;
	void AllocateCommandBuffer(VkCommandBuffer&, VkCommandPool);
	void RecordCommandBuffer(VkCommandBuffer, uint32_t);
	void FreeCommandBuffer(VkCommandBuffer, VkCommandPool);

	// Synchronization Members / Functions
	VkSemaphore mImageAvailable;
	VkSemaphore mRenderFinished;
	VkFence mInFlight;
	void CreateSyncObjects();
	void DestroySyncObjects();
};
#endif