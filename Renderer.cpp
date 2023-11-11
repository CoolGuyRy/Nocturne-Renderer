#include "Renderer.h"

#include <iostream>
#include <Windows.h>
#include "globals.h"

Renderer::Renderer() {
	CreateGLFWWindow();
	CreateInstance();
	CreateSurface();
	RetrievePhysicalDevice();
	CreateLogicalDevice();
	CreateSwapchain();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFramebuffers();
	CreateCommandPool(mGraphicsCommandPool, mQueueFamilyIndices.mGraphics);
	CreateCommandPool(mTransferCommandPool, mQueueFamilyIndices.mTransfer);
	CreateVertexBuffer();
	AllocateCommandBuffers(mGraphicsCommandPool);
	CreateSyncObjects();
}
Renderer::~Renderer() {
	vkDeviceWaitIdle(mLogicalDevice);
	// Delete in reverse order
	DestroySyncObjects();
	FreeCommandBuffers(mGraphicsCommandPool);
	DestroyVertexBuffer();
	DestroyCommandPool(mTransferCommandPool);
	DestroyCommandPool(mGraphicsCommandPool);
	DestroyFrameBuffers();
	DestroyGraphicsPipeline();
	DestroyRenderPass();
	DestroySwapchain();
	DestroyLogicalDevice();
	DestroySurface();
	DestroyInstance();
	DestroyGLFWWindow();
}
void Renderer::Run() {
	double deltaTime = 0.0, lastTime = 0.0, now = 0.0;
	while (!glfwWindowShouldClose(mWindow)) {
		now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		Input();
		Update(deltaTime);
		Render();
	}
}

void Renderer::Input() {
	glfwPollEvents();
}
void Renderer::Update(double deltaTime) {
	glfwSetWindowTitle(mWindow, (APPLICATION_TITLE + " | FPS: " + std::to_string(int(1.0 / deltaTime))).c_str());
}
void Renderer::Render() {
	vkWaitForFences(mLogicalDevice, 1, &mInFlight.at(mCurrentFrame), VK_TRUE, UINT64_MAX);
	vkResetFences(mLogicalDevice, 1, &mInFlight.at(mCurrentFrame));

	uint32_t imageIndex;
	vkAcquireNextImageKHR(mLogicalDevice, mSwapchain, UINT64_MAX, mImageAvailable.at(mCurrentFrame), VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(mCommandBuffers.at(mCurrentFrame), 0);
	RecordCommandBuffer(mCommandBuffers.at(mCurrentFrame), imageIndex);

	std::vector<VkPipelineStageFlags> waitStages = {
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	};
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &mImageAvailable.at(mCurrentFrame),
		.pWaitDstStageMask = waitStages.data(),
		.commandBufferCount = 1,
		.pCommandBuffers = &mCommandBuffers.at(mCurrentFrame),
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = &mRenderFinished.at(mCurrentFrame)
	};
	VkResult result = vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mInFlight.at(mCurrentFrame));
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer. Error Code: " + NT_CHECK_RESULT(result));
	}

	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.pNext = nullptr,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &mRenderFinished.at(mCurrentFrame),
		.swapchainCount = 1,
		.pSwapchains = &mSwapchain,
		.pImageIndices = &imageIndex,
		.pResults = nullptr
	};
	result = vkQueuePresentKHR(mPresentQueue, &presentInfo);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present image to swapchain. Error Code: " + NT_CHECK_RESULT(result));
	}

	mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::CreateGLFWWindow() {
	if (glfwInit()) {
		std::cout << "Success: GLFW initialized." << std::endl;
	} else {
		throw std::runtime_error("Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APPLICATION_TITLE.c_str(), nullptr, nullptr);

	if (mWindow) {
		std::cout << "Success: Window created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a GLFW Window!");
	}

	RECT desktopRect;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktopRect);

	int middleX = desktopRect.right / 2 - WINDOW_WIDTH / 2;
	int middleY = desktopRect.bottom / 2 - WINDOW_HEIGHT / 2;

	glfwSetWindowPos(mWindow, middleX, middleY);
}
void Renderer::DestroyGLFWWindow() {
	glfwDestroyWindow(mWindow); std::cout << "Success: Window destroyed." << std::endl;
	glfwTerminate(); std::cout << "Success: GLFW terminated." << std::endl;
}

void Renderer::CreateInstance() {
	// Set up Application Info Struct
	VkApplicationInfo applicationInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = APPLICATION_TITLE.c_str(),
		.applicationVersion = APPLICATION_VERSION,
		.pEngineName = ENGINE_TITLE.c_str(),
		.engineVersion = ENGINE_VERSION,
		.apiVersion = VULKAN_API_VERSION
	};

	// Grab GLFW Extensions
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// Push to Instance Level Extensions List
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		mExtensions.push_back(glfwExtensions[i]);
	}

	// Add any extra requested Instance Level Extensions here
	for (size_t i = 0; i < ENABLED_INSTANCE_EXTENSIONS.size(); i++) {
		mExtensions.push_back(ENABLED_INSTANCE_EXTENSIONS.at(i));
	}

	// Check if requested Instance Level Extensions are supported
	if (!CheckInstanceExtensionSupport()) {
		throw std::runtime_error("Failed to find required extensions!");
	}

	// Check if requested Instance Level Layers are supported
	if (ENABLE_VALIDATION_LAYERS && !CheckValidationLayerSupport()) {
		throw std::runtime_error("Failed to find requested Validation Layers!");
	}

	// Set Up Instance Create Info
	VkInstanceCreateInfo instanceCI = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.pApplicationInfo = &applicationInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = (uint32_t)mExtensions.size(),
		.ppEnabledExtensionNames = mExtensions.data()
	};

	// Add Validation Layers if enabled globally
	if (ENABLE_VALIDATION_LAYERS) {
		instanceCI.enabledLayerCount = (uint32_t)ENABLED_VALIDATION_LAYERS.size();
		instanceCI.ppEnabledLayerNames = ENABLED_VALIDATION_LAYERS.data();
	}

	// Create Instance
	VkResult result = vkCreateInstance(&instanceCI, nullptr, &mInstance);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Instance created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a VkInstance! Error Code: " + NT_CHECK_RESULT(result)); // not tested
	}
}
void Renderer::DestroyInstance() {
	vkDestroyInstance(mInstance, nullptr); std::cout << "Success: Instance destroyed." << std::endl;
}
bool Renderer::CheckInstanceExtensionSupport() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

	for (size_t i = 0; i < mExtensions.size(); i++) {
		bool found = false;
		for (uint32_t j = 0; j < extensionCount; j++) {
			if (strcmp(mExtensions.at(i), availableExtensions.at(j).extensionName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}

	return true;
}
bool Renderer::CheckValidationLayerSupport() {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (size_t i = 0; i < ENABLED_VALIDATION_LAYERS.size(); i++) {
		bool found = false;
		for (uint32_t j = 0; j < availableLayers.size(); j++) {
			if (strcmp(ENABLED_VALIDATION_LAYERS.at(i), availableLayers.at(j).layerName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}

	return true;
}

void Renderer::CreateSurface() {
	VkResult result = glfwCreateWindowSurface(mInstance, mWindow, nullptr, &mSurface);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Surface created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create surface! Error Code: " + NT_CHECK_RESULT(result));
	}
}
void Renderer::DestroySurface() {
	vkDestroySurfaceKHR(mInstance, mSurface, nullptr); std::cout << "Success: Surface destroyed." << std::endl;
}
void Renderer::AcquireSurfaceProperties(VkPhysicalDevice pDevice) {
	// Get surface properties
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, mSurface, &mSurfaceCapabilities);

	// Get surface formats
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, mSurface, &formatCount, nullptr);
	mAvailableSurfaceFormats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, mSurface, &formatCount, mAvailableSurfaceFormats.data());

	// Get surface present modes
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, mSurface, &presentModeCount, nullptr);
	mAvailablePresentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, mSurface, &presentModeCount, mAvailablePresentModes.data());

	// Hard-coded values
	mBestSurfaceFormat = mAvailableSurfaceFormats.at(0);
	mBestPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
}

void Renderer::RetrievePhysicalDevice() {
	// Get the number of physical devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	// If there are no devices with Vulkan support, throw an error
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");
	}

	// Create a list of physical devices
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	for (size_t i = 0; i < devices.size(); i++) {
		if (!CheckDeviceSuitable(devices.at(i))) {
			continue;
		}
		if (!CheckDeviceExtensionSupport(devices.at(i))) {
			continue;
		}
		mPhysicalDevice = devices.at(i);
		break;
	}

	// Check if a suitable device was found
	if (mPhysicalDevice != nullptr) {
		std::cout << "Success: Suitable Physical Device located." << std::endl;
	} else {
		throw std::runtime_error("Failed to locate a suitable Physical Device!");
	}

	AssignQueueFamilyIndices();
	ValidateQueueFamilyIndices();
}
bool Renderer::CheckDeviceSuitable(VkPhysicalDevice pDevice) {
	vkGetPhysicalDeviceProperties(pDevice, &mPhysicalDeviceProperties);
	vkGetPhysicalDeviceFeatures(pDevice, &mPhysicalDeviceFeatures);
	vkGetPhysicalDeviceMemoryProperties(pDevice, &mPhysicalDeviceMemoryProperties);

	switch (mPhysicalDeviceProperties.deviceType) {
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
		return true;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
		return true;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		return true;
		break;
	case VK_PHYSICAL_DEVICE_TYPE_CPU:
		return true;
		break;
	default:
		return false;
		mPhysicalDeviceProperties = { };
		mPhysicalDeviceFeatures = { };
		mPhysicalDeviceMemoryProperties = { };
		break;
	}
}
bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice pDevice) {
	// Get the number of extensions
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);

	// Create a list of extensions
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());

	// Check if the required extensions are in the list of available extensions
	for (size_t i = 0; i < ENABLED_DEVICE_EXTENSIONS.size(); i++) {
		bool found = false;
		for (uint32_t j = 0; j < availableExtensions.size(); j++) {
			if (strcmp(ENABLED_DEVICE_EXTENSIONS.at(i), availableExtensions.at(j).extensionName) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			return false;
		}
	}

	return true;
}
void Renderer::AssignQueueFamilyIndices() {
	// Get the number of queue families
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);

	// Create a list of queue families
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, queueFamilies.data());

	// Iterate through Queue Family Properties and assign the indices
	for (uint32_t i = 0; i < queueFamilies.size(); i++) {
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_GRAPHICS_BIT && queueFamilies.at(i).queueCount > 0) {
			mQueueFamilyIndices.mGraphics = i;
		}
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_COMPUTE_BIT && queueFamilies.at(i).queueCount > 0) {
			mQueueFamilyIndices.mCompute = i;
		}
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_TRANSFER_BIT && queueFamilies.at(i).queueCount > 0) {
			mQueueFamilyIndices.mTransfer = i;
		}
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_SPARSE_BINDING_BIT && queueFamilies.at(i).queueCount > 0) {
			mQueueFamilyIndices.mSparseBinding = i;
		}

		// This code checks to see if the queue family supports presentation. But we set a preference for the graphics queue.
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, i, mSurface, &presentSupport);
		if (presentSupport && mQueueFamilyIndices.mPresent == -1) {
			mQueueFamilyIndices.mPresent = i;
		}
	}
}
void Renderer::ValidateQueueFamilyIndices() {
	if (mQueueFamilyIndices.mGraphics == -1) {
		throw std::runtime_error("Failed to find a suitable queue family for graphics operations!");
	}
	if (mQueueFamilyIndices.mPresent == -1) {
		throw std::runtime_error("Failed to find a suitable queue family for present operations");
	}
	if (mQueueFamilyIndices.mCompute == -1) {
		throw std::runtime_error("Failed to find a suitable queue family for compute operations!");
	}
	if (mQueueFamilyIndices.mTransfer == -1) {
		throw std::runtime_error("Failed to find a suitable queue family for transfer operations!");
	}
	if (mQueueFamilyIndices.mSparseBinding == -1) {
		throw std::runtime_error("Failed to find a suitable queue family for sparse binding operations!");
	}
}

void Renderer::CreateLogicalDevice() {
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueCI = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)mQueueFamilyIndices.mGraphics,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};
	VkDeviceQueueCreateInfo presentQueueCI = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)mQueueFamilyIndices.mPresent,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};
	VkDeviceQueueCreateInfo transferQueueCI = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = (uint32_t)mQueueFamilyIndices.mTransfer,
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};

	// Create an array to pass to deviceCI
	std::vector<VkDeviceQueueCreateInfo> queueCIs = { graphicsQueueCI, transferQueueCI };

	// Check if the present queue is the same as the graphics queue
	if (mQueueFamilyIndices.mGraphics != mQueueFamilyIndices.mPresent) {
		queueCIs.push_back(presentQueueCI);
	}

	// Describe the logical device to be created
	VkDeviceCreateInfo deviceCI = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueCreateInfoCount = (uint32_t)queueCIs.size(),
		.pQueueCreateInfos = queueCIs.data(),
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = nullptr,
		.enabledExtensionCount = (uint32_t)ENABLED_DEVICE_EXTENSIONS.size(),
		.ppEnabledExtensionNames = ENABLED_DEVICE_EXTENSIONS.data(),
		.pEnabledFeatures = &ENABLED_PHYSICAL_DEVICE_FEATURES
	};

	// Create the logical device
	VkResult result = vkCreateDevice(mPhysicalDevice, &deviceCI, nullptr, &mLogicalDevice);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Logical Device created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a Logical Device! Error Code: " + NT_CHECK_RESULT(result)); // not tested
	}

	// Get the created queue handles
	vkGetDeviceQueue(mLogicalDevice, mQueueFamilyIndices.mGraphics, 0, &mGraphicsQueue);
	vkGetDeviceQueue(mLogicalDevice, mQueueFamilyIndices.mPresent, 0, &mPresentQueue);
	vkGetDeviceQueue(mLogicalDevice, mQueueFamilyIndices.mTransfer, 0, &mTransferQueue);
}
void Renderer::DestroyLogicalDevice() {
	vkDestroyDevice(mLogicalDevice, nullptr); std::cout << "Success: Logical Device destroyed." << std::endl;
}

void Renderer::CreateSwapchain() {
	// Ask Surface to Provide Swapchain Details
	AcquireSurfaceProperties(mPhysicalDevice);

	// Describe the swapchain
	VkSwapchainCreateInfoKHR swapchainCI = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.pNext = nullptr,
		.flags = 0,
		.surface = mSurface,
		.minImageCount = 3,
		.imageFormat = mBestSurfaceFormat.format,
		.imageColorSpace = mBestSurfaceFormat.colorSpace,
		.imageExtent = {
			.width = WINDOW_WIDTH,
			.height = WINDOW_HEIGHT
		},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = mBestPresentMode,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE
	};

	// Check if the queue families are the same
	std::vector<uint32_t> queueFamilyIndices;
	queueFamilyIndices.push_back(mQueueFamilyIndices.mGraphics);
	if (mQueueFamilyIndices.mGraphics == mQueueFamilyIndices.mPresent) {
		swapchainCI.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();
		swapchainCI.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		queueFamilyIndices.push_back(mQueueFamilyIndices.mPresent);
		swapchainCI.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();
		swapchainCI.pQueueFamilyIndices = queueFamilyIndices.data();
		swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	}

	// Create the swapchain
	VkResult result = vkCreateSwapchainKHR(mLogicalDevice, &swapchainCI, nullptr, &mSwapchain);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Swapchain created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create swapchain! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Establish the Swapchain Extent
	mSwapchainExtent = {
		.width = (uint32_t)WINDOW_WIDTH,
		.height = (uint32_t)WINDOW_HEIGHT
	};

	// Grab the Swapchain Images
	uint32_t swapchainImagesCount = 0;
	vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &swapchainImagesCount, nullptr);
	std::vector<VkImage> swapchainImages(swapchainImagesCount);
	vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &swapchainImagesCount, swapchainImages.data());

	// Create ImageViews for each Swapchain Image and push to mSwapchainImages vector
	for (size_t i = 0; i < swapchainImages.size(); i++) {
		mSwapchainImages.push_back({ swapchainImages.at(i), new ImageView(mLogicalDevice, swapchainImages.at(i), mBestSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT)});
	}
}
void Renderer::DestroySwapchain() {
	for (size_t i = 0; i < mSwapchainImages.size(); i++) {
		delete mSwapchainImages.at(i).mImageView;
	}
	vkDestroySwapchainKHR(mLogicalDevice, mSwapchain, nullptr); std::cout << "Success: Swapchain destroyed." << std::endl;
}

void Renderer::CreateRenderPass() {
	// Attachment Descriptions
	// - Color Attachment
	VkAttachmentDescription colorAttachment {
		.flags = 0,
		.format = mBestSurfaceFormat.format,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	// Put Attachments together in a vector
	std::vector<VkAttachmentDescription> attachmentDescriptions = { colorAttachment };

	// Attachment References
	// - Color Attachment Reference
	VkAttachmentReference colorAttachmentRef {
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	// Subpass Descriptions
	// - First Subpass
	VkSubpassDescription firstSubpass {
		.flags = 0,
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.inputAttachmentCount = 0,
		.pInputAttachments = nullptr,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentRef,
		.pResolveAttachments = nullptr,
		.pDepthStencilAttachment = nullptr,
		.preserveAttachmentCount = 0,
		.pPreserveAttachments = nullptr
	};

	// Put Subpasses together in a vector
	std::vector<VkSubpassDescription> subpasses = { firstSubpass };

	// Subpass Dependencies
	// - - Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	VkSubpassDependency firstSubpassDependency {
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = 0,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		.dependencyFlags = 0
	};

	// Put Subpass Dependencies in a vector
	std::vector<VkSubpassDependency> subpassDependencies = { firstSubpassDependency };

	// Render Pass create info structure
	VkRenderPassCreateInfo renderPassCI = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.attachmentCount = (uint32_t)attachmentDescriptions.size(),
		.pAttachments = attachmentDescriptions.data(),
		.subpassCount = (uint32_t)subpasses.size(),
		.pSubpasses = subpasses.data(),
		.dependencyCount = (uint32_t)subpassDependencies.size(),
		.pDependencies = subpassDependencies.data()
	};

	// Create Render Pass
	VkResult result = vkCreateRenderPass(mLogicalDevice, &renderPassCI, nullptr, &mRenderPass);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Render Pass created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create Render Pass! Error Code: " + NT_CHECK_RESULT(result));
	}
}
void Renderer::DestroyRenderPass() {
	vkDestroyRenderPass(mLogicalDevice, mRenderPass, nullptr); std::cout << "Success: Render Pass destroyed." << std::endl;
}

void Renderer::CreateGraphicsPipeline() {
	// Grab the shader file locations
	std::vector<std::string> shaderFileNames = {
		".\\Resources\\Shaders\\simple.vert.spv",
		".\\Resources\\Shaders\\simple.frag.spv"
	};

	// Initialize ShaderWrapper classes
	std::vector<Shader*> mShaders(shaderFileNames.size());
	for (size_t i = 0; i < shaderFileNames.size(); i++) {
		mShaders.at(i) = new Shader(mLogicalDevice, shaderFileNames[i]);
	}

	// Create the shader stage create info structs
	std::vector<VkPipelineShaderStageCreateInfo> shaderStageCIs(mShaders.size());
	for (size_t i = 0; i < mShaders.size(); i++) {
		shaderStageCIs.at(i) = mShaders.at(i)->GetShaderCI();
	}

	// Grab Vertex Description Info from Vertex Class
	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescription = Vertex::GetAttributeDescriptions();

	// Create the vertex input state create info struct
	VkPipelineVertexInputStateCreateInfo vertexInputCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &bindingDescription,
		.vertexAttributeDescriptionCount = (uint32_t)attributeDescription.size(),
		.pVertexAttributeDescriptions = attributeDescription.data()
	};

	// Create the input assembly state create info struct
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	// Create the viewport state create info struct
	VkViewport viewport = {
		.x = 0.0f,
		.y = 0.0f,
		.width = (float)WINDOW_WIDTH,
		.height = (float)WINDOW_HEIGHT,
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};
	VkRect2D scissor{
		.offset = {
			.x = 0,
			.y = 0
		},
		.extent = {
			.width = WINDOW_WIDTH, 
			.height = WINDOW_HEIGHT
		}
	};
	VkPipelineViewportStateCreateInfo viewportCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
	};

	// Create the rasterization state create info struct
	VkPipelineRasterizationStateCreateInfo rasterizerCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.lineWidth = 1.0f
	};

	// Create the multisample state create info struct
	VkPipelineMultisampleStateCreateInfo multiSamplingCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
		.minSampleShading = 1.0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE
	};

	// Create the depth stencil state create info struct
	// TODO: Implement later

	// Create the color blend attachment state struct
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {
		.blendEnable = VK_TRUE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};
	VkPipelineColorBlendStateCreateInfo colorBlendCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
		.blendConstants = { 
			0.0f, 0.0f, 0.0f, 0.0f
		}
	};

	// Create the pipeline layout create info struct
	VkPipelineLayoutCreateInfo pipelineLayoutCI = {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr
	};

	// Create the pipeline layout
	VkResult result = vkCreatePipelineLayout(mLogicalDevice, &pipelineLayoutCI, nullptr, &mPipelineLayout);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Pipeline layout created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a pipeline layout! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Create the graphics pipeline create info struct
	VkGraphicsPipelineCreateInfo graphicsPipelineCI = {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.stageCount = (uint32_t)shaderStageCIs.size(),
		.pStages = shaderStageCIs.data(),
		.pVertexInputState = &vertexInputCI,
		.pInputAssemblyState = &inputAssemblyCI,
		.pTessellationState = nullptr,
		.pViewportState = &viewportCI,
		.pRasterizationState = &rasterizerCI,
		.pMultisampleState = &multiSamplingCI,
		.pDepthStencilState = nullptr,
		.pColorBlendState = &colorBlendCI,
		.pDynamicState = nullptr,
		.layout = mPipelineLayout,
		.renderPass = mRenderPass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1
	};

	result = vkCreateGraphicsPipelines(mLogicalDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCI, nullptr, &mPipeline);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Graphics pipeline created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create graphics pipeline! Error Code: " + NT_CHECK_RESULT(result));
	}

	// Destroy Shader Modules, since graphics pipeline has been created
	for (size_t i = 0; i < mShaders.size(); i++) {
		vkDestroyShaderModule(mLogicalDevice, mShaders.at(i)->GetShaderModule(), nullptr);	std::cout << "Success: Shader module destroyed." << std::endl;
	}
}
void Renderer::DestroyGraphicsPipeline() {
	vkDestroyPipelineLayout(mLogicalDevice, mPipelineLayout, nullptr);	std::cout << "Success: Pipeline Layout Destroyed." << std::endl;
	vkDestroyPipeline(mLogicalDevice, mPipeline, nullptr);	std::cout << "Success: Pipeline Destroyed." << std::endl;
}

void Renderer::CreateFramebuffers() {
	mFramebuffers.resize(mSwapchainImages.size());

	for (size_t i = 0; i < mSwapchainImages.size(); i++) {
		std::vector<VkImageView> attachments = {
			mSwapchainImages[i].mImageView->GetImageView()
		};

		VkFramebufferCreateInfo framebufferCI = {
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.renderPass = mRenderPass,
			.attachmentCount = (uint32_t)attachments.size(),
			.pAttachments = attachments.data(),
			.width = WINDOW_WIDTH,
			.height = WINDOW_HEIGHT,
			.layers = 1
		};

		VkResult result = vkCreateFramebuffer(mLogicalDevice, &framebufferCI, nullptr, &mFramebuffers.at(i));
		if (result == VK_SUCCESS) {
			std::cout << "Success: Framebuffer created." << std::endl;
		} else {
			throw std::runtime_error("Failed to create framebuffer! Error Code: " + NT_CHECK_RESULT(result));
		}
	}
}
void Renderer::DestroyFrameBuffers() {
	for (auto fb : mFramebuffers) {
		vkDestroyFramebuffer(mLogicalDevice, fb, nullptr); std::cout << "Success: Framebuffer destroyed." << std::endl;
	}
}

void Renderer::CreateCommandPool(VkCommandPool& pool, uint32_t index) {
	VkCommandPoolCreateInfo poolCI = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.pNext = nullptr,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = index
	};

	VkResult result = vkCreateCommandPool(mLogicalDevice, &poolCI, nullptr, &pool);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Command pool created" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Command Pool! Error Code: " + NT_CHECK_RESULT(result));
	}
}
void Renderer::DestroyCommandPool(VkCommandPool pool) {
	vkDestroyCommandPool(mLogicalDevice, pool, nullptr); std::cout << "Success: Command pool destroyed." << std::endl;
}

void Renderer::AllocateCommandBuffers(VkCommandPool pool) {
	mCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo commandBufferAI = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.pNext = nullptr,
		.commandPool = pool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = (uint32_t)mCommandBuffers.size()
	};

	VkResult result = vkAllocateCommandBuffers(mLogicalDevice, &commandBufferAI, mCommandBuffers.data());
	if (result == VK_SUCCESS) {
		std::cout << "Success: Command Buffer allocated." << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}
}
void Renderer::RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex) {
	VkCommandBufferBeginInfo beginInfo = {
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0,
		.pInheritanceInfo = nullptr
	};
	VkResult result = vkBeginCommandBuffer(buffer, &beginInfo);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

		VkClearValue clearColor = { 
			.color = {
				0.11f, 0.24f, 0.36f, 1.0f
			},
		};
		VkRenderPassBeginInfo renderPassBI = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.pNext = nullptr,
			.renderPass = mRenderPass,
			.framebuffer = mFramebuffers.at(imageIndex),
			.renderArea = {
				.offset = {
					.x = 0,
					.y = 0
				},
				.extent = mSwapchainExtent
			},
			.clearValueCount = 1,
			.pClearValues = &clearColor
		};
		vkCmdBeginRenderPass(buffer, &renderPassBI, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);

			VkBuffer vertexBuffers[] = { mVertexBuffer };
			VkDeviceSize offsets[] = { 0 };

			vkCmdBindVertexBuffers(mCommandBuffers.at(mCurrentFrame), 0, 1, vertexBuffers, offsets);

			vkCmdDraw(buffer, (uint32_t)vertices.size(), 1, 0, 0);

		vkCmdEndRenderPass(buffer);

	result = vkEndCommandBuffer(buffer);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to end record command buffer! Error Code: " + NT_CHECK_RESULT(result));
	}
}
void Renderer::FreeCommandBuffers(VkCommandPool pool) {
	vkFreeCommandBuffers(mLogicalDevice, pool, (uint32_t)mCommandBuffers.size(), mCommandBuffers.data()); std::cout << "Success: Command Buffer freed." << std::endl;
}

void Renderer::CreateSyncObjects() {
	VkSemaphoreCreateInfo semaphoreCI = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0
	};
	VkFenceCreateInfo fenceCI = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT
	};

	mImageAvailable.resize(MAX_FRAMES_IN_FLIGHT);
	mRenderFinished.resize(MAX_FRAMES_IN_FLIGHT);
	mInFlight.resize(MAX_FRAMES_IN_FLIGHT);

	for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VkResult result = vkCreateSemaphore(mLogicalDevice, &semaphoreCI, nullptr, &mImageAvailable.at(i));
		if (result == VK_SUCCESS) {
			std::cout << "Success: Semaphore created." << std::endl;
		} else {
			throw std::runtime_error("Failed to create a semaphore! Error Code: " + NT_CHECK_RESULT(result));
		}

		result = vkCreateSemaphore(mLogicalDevice, &semaphoreCI, nullptr, &mRenderFinished.at(i));
		if (result == VK_SUCCESS) {
			std::cout << "Success: Semaphore created." << std::endl;
		} else {
			throw std::runtime_error("Failed to create a semaphore! Error Code: " + NT_CHECK_RESULT(result));
		}

		result = vkCreateFence(mLogicalDevice, &fenceCI, nullptr, &mInFlight.at(i));
		if (result == VK_SUCCESS) {
			std::cout << "Success: Fence created." << std::endl;
		} else {
			throw std::runtime_error("Failed to create fence! Error Code: " + NT_CHECK_RESULT(result));
		}
	}
}
void Renderer::DestroySyncObjects() {
	for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyFence(mLogicalDevice, mInFlight.at(i), nullptr); std::cout << "Success: Fence destroyed." << std::endl;
		vkDestroySemaphore(mLogicalDevice, mRenderFinished.at(i), nullptr); std::cout << "Success: Semaphore destroyed." << std::endl;
		vkDestroySemaphore(mLogicalDevice, mImageAvailable.at(i), nullptr); std::cout << "Success: Semaphore destroyed." << std::endl;
	}
}

void Renderer::CreateVertexBuffer() {
	VkBufferCreateInfo bufferCI = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.size = sizeof(vertices[0]) * vertices.size(),
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices = nullptr
	};

	VkResult result = vkCreateBuffer(mLogicalDevice, &bufferCI, nullptr, &mVertexBuffer);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Buffer created!" << std::endl;
	} else {
		throw std::runtime_error("Failed to create Buffer! Error Code: " + NT_CHECK_RESULT(result));
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(mLogicalDevice, mVertexBuffer, &memRequirements);

	VkMemoryAllocateInfo memoryAI = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext = nullptr,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FindMemoryTypeIndex(mPhysicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};

	result = vkAllocateMemory(mLogicalDevice, &memoryAI, nullptr, &mVertexBufferMemory);
	if (result == VK_SUCCESS) {
		std::cout << "Success: Buffer Memory allocated!" << std::endl;
	} else {
		throw std::runtime_error("Failed to allocate Buffer Memory! Error Code: " + NT_CHECK_RESULT(result));
	}

	vkBindBufferMemory(mLogicalDevice, mVertexBuffer, mVertexBufferMemory, 0);

	void* data;
	vkMapMemory(mLogicalDevice, mVertexBufferMemory, 0, bufferCI.size, 0, &data);
		memcpy(data, vertices.data(), (size_t)bufferCI.size);
	vkUnmapMemory(mLogicalDevice, mVertexBufferMemory);
}
void Renderer::DestroyVertexBuffer() {
	vkDestroyBuffer(mLogicalDevice, mVertexBuffer, nullptr); std::cout << "Success: Buffer destroyed." << std::endl;
	vkFreeMemory(mLogicalDevice, mVertexBufferMemory, nullptr); std::cout << "Success: Buffer Memory freed." << std::endl;
}
