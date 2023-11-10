#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ImageView.h"
#include "Shader.h"

class Vertex {
public:
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {
			.binding = 0,
			.stride = sizeof(Vertex),
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		};

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions {};

		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

class Renderer {
public:
	Renderer();
	~Renderer();

	void Run();
private:
	// Top-Level Resources (Textures, Models, etc)
	const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	// Run() Support Functions
	unsigned mCurrentFrame;
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
	std::vector<VkCommandBuffer> mCommandBuffers;
	void AllocateCommandBuffers(VkCommandPool);
	void RecordCommandBuffer(VkCommandBuffer, uint32_t);
	void FreeCommandBuffers(VkCommandPool);

	// Synchronization Members / Functions
	std::vector<VkSemaphore> mImageAvailable;
	std::vector<VkSemaphore> mRenderFinished;
	std::vector<VkFence> mInFlight;
	void CreateSyncObjects();
	void DestroySyncObjects();
};
#endif