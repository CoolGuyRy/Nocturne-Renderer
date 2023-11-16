#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ImageView.h"
#include "Shader.h"
#include "Buffer.h"

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
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {
			{
				{
					.location = 0,
					.binding = 0,
					.format = VK_FORMAT_R32G32_SFLOAT,
					.offset = offsetof(Vertex, pos)
				},
				{
					.location = 1,
					.binding = 0,
					.format = VK_FORMAT_R32G32B32_SFLOAT,
					.offset = offsetof(Vertex, color)
				}
			}
		};

		return attributeDescriptions;
	}
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Renderer {
public:
	Renderer();
	~Renderer();

	void Run();
	// Top-Level Resources (Textures, Models, etc)
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
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

	void AllocateCommandBuffer(VkCommandBuffer&, VkCommandPool);
	void FreeCommandBuffer(VkCommandBuffer, VkCommandPool);

	void AllocateGraphicsCommandBuffers();
	void RecordCommandBuffer(VkCommandBuffer, uint32_t);
	void FreeCommandBuffers(VkCommandPool);

	// Synchronization Members / Functions
	std::vector<VkSemaphore> mImageAvailable;
	std::vector<VkSemaphore> mRenderFinished;
	std::vector<VkFence> mInFlight;
	void CreateSyncObjects();
	void DestroySyncObjects();

	// Buffer Members / Functions
	Buffer* mVertexBuffer;
	void CreateVertexBuffer();
	void DestroyVertexBuffer();

	Buffer* mIndexBuffer;
	void CreateIndexBuffer();
	void DestroyIndexBuffer();

	// Descriptor Members / Functions
	VkDescriptorSetLayout mDescriptorSetLayout;
	void CreateDescriptorSetLayout();
	void DestroyDescriptorSetLayout();

	// Uniform Buffers Members / Functions
	std::vector<Buffer*> mUniformBuffers;
	std::vector<void*> mUniformBufferData;
	void CreateUniformBuffers();
	void DestroyUniformBuffers();
	void UpdateUniformBuffer(uint32_t);

	// Descriptor Pool Members / Functions
	VkDescriptorPool mDescriptorPool;
	void CreateDescriptorPool();
	void DestroyDescriptorPool();

	// Descriptor Set Members / Functions
	std::vector<VkDescriptorSet> mDescriptorSets;
	void CreateDescriptorSets();
};
#endif