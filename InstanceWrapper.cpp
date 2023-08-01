#include "InstanceWrapper.h"

InstanceWrapper::InstanceWrapper() {
	CreateInstance();
}

InstanceWrapper::~InstanceWrapper() {
	vkDestroyInstance(mInstance, nullptr); std::cout << "Success: Instance Destroyed." << std::endl;
}

VkInstance InstanceWrapper::GetInstance() {
	return mInstance;
}

void InstanceWrapper::CreateInstance() {
	// Set up Application Info Struct
	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,				// sType
		nullptr,										// pNext
		APPLICATION_TITLE.c_str(),						// pApplicationName
		APPLICATION_VERSION,							// applicationVersion
		ENGINE_TITLE.c_str(),							// pEngineName
		ENGINE_VERSION,									// engineVersion
		VULKAN_API_VERSION								// apiVersion
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


	// Check if requested Instance Level Extensions are supported
	if (!checkInstanceExtensionSupport()) {
		throw std::runtime_error("Failed to find required extensions!");
	}

	// Check if requested Instance Level Layers are supported
	if (ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport()) {
		throw std::runtime_error("Failed to find requested Validation Layers!");
	}

	// Set Up Instance Create Info
	VkInstanceCreateInfo instanceCI = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,			// sType
		nullptr,										// pNext
		0,												// flags
		&applicationInfo,								// pApplicationInfo
		0,												// enabledLayerCount
		nullptr,										// ppEnabledLayerNames
		(uint32_t)mExtensions.size(),					// enabledExtensionCount
		mExtensions.data()								// ppEnabledExtensionNames
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

/*

	This function goes through the mExtensions list and checks to see if they are supported by the Vulkan Instance.

*/
bool InstanceWrapper::checkInstanceExtensionSupport() {
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

/*

	This function is similar to the checkInstanceExtensionSupport() function, but it checks for the Validation Layers instead.

*/
bool InstanceWrapper::checkValidationLayerSupport() {
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