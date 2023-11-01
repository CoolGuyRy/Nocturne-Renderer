#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>

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

	// Window Member / Functions
	GLFWwindow* mWindow;
	void CreateGLFWWindow();
	void DestroyGLFWWindow();

	// Instance Member / Functions
	VkInstance mInstance;
	std::vector<const char*> mExtensions;
	void CreateInstance();
	void DestroyInstance();
	bool CheckInstanceExtensionSupport();
	bool CheckValidationLayerSupport();

	// Surface Member / Functions
	VkSurfaceKHR mSurface;
	VkSurfaceCapabilitiesKHR mSurfaceCapabilities;
	VkSurfaceFormatKHR mBestSurfaceFormat;
	VkPresentModeKHR mBestPresentMode;
	std::vector<VkSurfaceFormatKHR> mAvailableSurfaceFormats;
	std::vector<VkPresentModeKHR> mAvailablePresentModes;
	void CreateSurface();
	void DestroySurface();
	void AcquireSurfaceProperties(VkPhysicalDevice);

	// Physical Device Member / Functions
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

	// Logical Device Member / Functions
	VkDevice mLogicalDevice;
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;
	VkQueue mTransferQueue;
	void CreateLogicalDevice();
	void DestroyLogicalDevice();
	bool CheckDeviceExtensionSupport();
};
#endif