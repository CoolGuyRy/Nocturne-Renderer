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
}
Renderer::~Renderer() {
	// Delete in reverse order
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

}
void Renderer::Render() {

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
