#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> ENABLED_VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation",
	//"VK_LAYER_LUNARG_api_dump",
	//"VK_LAYER_LUNARG_gfxreconstruct",
	//"VK_LAYER_KHRONOS_synchronization2",
};
const std::vector<const char*> ENABLED_INSTANCE_EXTENSIONS = {

};
const std::vector<const char*> ENABLED_DEVICE_EXTENSIONS = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const VkPhysicalDeviceFeatures ENABLED_PHYSICAL_DEVICE_FEATURES = {
	VK_FALSE, // VkBool32    robustBufferAccess;
	VK_FALSE, // VkBool32    fullDrawIndexUint32;
	VK_FALSE, // VkBool32    imageCubeArray;
	VK_FALSE, // VkBool32    independentBlend;
	VK_FALSE, // VkBool32    geometryShader;
	VK_FALSE, // VkBool32    tessellationShader;
	VK_FALSE, // VkBool32    sampleRateShading;
	VK_FALSE, // VkBool32    dualSrcBlend;
	VK_FALSE, // VkBool32    logicOp;
	VK_FALSE, // VkBool32    multiDrawIndirect;
	VK_FALSE, // VkBool32    drawIndirectFirstInstance;
	VK_FALSE, // VkBool32    depthClamp;
	VK_FALSE, // VkBool32    depthBiasClamp;
	VK_FALSE, // VkBool32    fillModeNonSolid;
	VK_FALSE, // VkBool32    depthBounds;
	VK_FALSE, // VkBool32    wideLines;
	VK_FALSE, // VkBool32    largePoints;
	VK_FALSE, // VkBool32    alphaToOne;
	VK_FALSE, // VkBool32    multiViewport;
	VK_FALSE, // VkBool32    samplerAnisotropy;
	VK_FALSE, // VkBool32    textureCompressionETC2;
	VK_FALSE, // VkBool32    textureCompressionASTC_LDR;
	VK_FALSE, // VkBool32    textureCompressionBC;
	VK_FALSE, // VkBool32    occlusionQueryPrecise;
	VK_FALSE, // VkBool32    pipelineStatisticsQuery;
	VK_FALSE, // VkBool32    vertexPipelineStoresAndAtomics;
	VK_FALSE, // VkBool32    fragmentStoresAndAtomics;
	VK_FALSE, // VkBool32    shaderTessellationAndGeometryPointSize;
	VK_FALSE, // VkBool32    shaderImageGatherExtended;
	VK_FALSE, // VkBool32    shaderStorageImageExtendedFormats;
	VK_FALSE, // VkBool32    shaderStorageImageMultisample;
	VK_FALSE, // VkBool32    shaderStorageImageReadWithoutFormat;
	VK_FALSE, // VkBool32    shaderStorageImageWriteWithoutFormat;
	VK_FALSE, // VkBool32    shaderUniformBufferArrayDynamicIndexing;
	VK_FALSE, // VkBool32    shaderSampledImageArrayDynamicIndexing;
	VK_FALSE, // VkBool32    shaderStorageBufferArrayDynamicIndexing;
	VK_FALSE, // VkBool32    shaderStorageImageArrayDynamicIndexing;
	VK_FALSE, // VkBool32    shaderClipDistance;
	VK_FALSE, // VkBool32    shaderCullDistance;
	VK_FALSE, // VkBool32    shaderFloat64;
	VK_FALSE, // VkBool32    shaderInt64;
	VK_FALSE, // VkBool32    shaderInt16;
	VK_FALSE, // VkBool32    shaderResourceResidency;
	VK_FALSE, // VkBool32    shaderResourceMinLod;
	VK_FALSE, // VkBool32    sparseBinding;
	VK_FALSE, // VkBool32    sparseResidencyBuffer;
	VK_FALSE, // VkBool32    sparseResidencyImage2D;
	VK_FALSE, // VkBool32    sparseResidencyImage3D;
	VK_FALSE, // VkBool32    sparseResidency2Samples;
	VK_FALSE, // VkBool32    sparseResidency4Samples;
	VK_FALSE, // VkBool32    sparseResidency8Samples;
	VK_FALSE, // VkBool32    sparseResidency16Samples;
	VK_FALSE, // VkBool32    sparseResidencyAliased;
	VK_FALSE, // VkBool32    variableMultisampleRate;
	VK_FALSE  // VkBool32    inheritedQueries;
};
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 960;
const std::string APPLICATION_TITLE = "Nocturne Renderer";
const std::string ENGINE_TITLE = "Nocturne Engine";
const uint32_t APPLICATION_VERSION = VK_MAKE_VERSION(1, 0, 0);
const uint32_t ENGINE_VERSION = VK_MAKE_VERSION(1, 0, 0);
const uint32_t VULKAN_API_VERSION = VK_API_VERSION_1_1;

static std::string NT_CHECK_RESULT(int code) {
	switch (code) {
	case VK_NOT_READY:
		return "VK_NOT_READY";
		break;
	case VK_TIMEOUT:
		return "VK_TIMEOUT";
		break;
	case VK_EVENT_SET:
		return "VK_EVENT_SET";
		break;
	case VK_EVENT_RESET:
		return "VK_EVENT_RESET";
		break;
	case VK_INCOMPLETE:
		return "VK_INCOMPLETE";
		break;
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "VK_ERROR_OUT_OF_HOST_MEMORY";
		break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		break;
	case VK_ERROR_INITIALIZATION_FAILED:
		return "VK_ERROR_INITIALIZATION_FAILED";
		break;
	case VK_ERROR_DEVICE_LOST:
		return "VK_ERROR_DEVICE_LOST";
		break;
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "VK_ERROR_MEMORY_MAP_FAILED";
		break;
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "VK_ERROR_LAYER_NOT_PRESENT";
		break;
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "VK_ERROR_EXTENSION_NOT_PRESENT";
		break;
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "VK_ERROR_FEATURE_NOT_PRESENT";
		break;
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "VK_ERROR_INCOMPATIBLE_DRIVER";
		break;
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "VK_ERROR_TOO_MANY_OBJECTS";
		break;
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		break;
	case VK_ERROR_FRAGMENTED_POOL:
		return "VK_ERROR_FRAGMENTED_POOL";
		break;
	case VK_ERROR_UNKNOWN:
		return "VK_ERROR_UNKNOWN";
		break;
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return "VK_ERROR_OUT_OF_POOL_MEMORY";
		break;
	case VK_ERROR_INVALID_EXTERNAL_HANDLE:
		return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
		break;
	case VK_ERROR_FRAGMENTATION:
		return "VK_ERROR_FRAGMENTATION";
		break;
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
		return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
		break;
	case VK_PIPELINE_COMPILE_REQUIRED:
		return "VK_PIPELINE_COMPILE_REQUIRED";
		break;
	case VK_ERROR_SURFACE_LOST_KHR:
		return "VK_ERROR_SURFACE_LOST_KHR";
		break;
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
		break;
	case VK_SUBOPTIMAL_KHR:
		return "VK_SUBOPTIMAL_KHR";
		break;
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "VK_ERROR_OUT_OF_DATE_KHR";
		break;
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
		break;
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "VK_ERROR_VALIDATION_FAILED_EXT";
		break;
	case VK_ERROR_INVALID_SHADER_NV:
		return "VK_ERROR_INVALID_SHADER_NV";
		break;
	case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
		return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
		break;
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
		break;
	case VK_ERROR_NOT_PERMITTED_EXT:
		return "VK_ERROR_NOT_PERMITTED_EXT";
		break;
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
		return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
		break;
	case VK_THREAD_IDLE_KHR:
		return "VK_THREAD_IDLE_KHR";
		break;
	case VK_THREAD_DONE_KHR:
		return "VK_THREAD_DONE_KHR";
		break;
	case VK_OPERATION_DEFERRED_KHR:
		return "VK_OPERATION_DEFERRED_KHR";
		break;
	case VK_OPERATION_NOT_DEFERRED_KHR:
		return "VK_OPERATION_NOT_DEFERRED_KHR";
		break;
	default:
		return "NOCTURNE_NOT_DEFINED";
		break;
	}
}

/*

	This function is used to find the memory type index that has all the required property bits set.
	It loops through all the memory types available on the device and checks if their bit field matches
	the desired properties. Once it finds a memory type that fits all the properties we need, it returns

*/
static uint32_t FindMemoryTypeIndex(VkPhysicalDevice device, uint32_t allowedTypes, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		if ((allowedTypes & (1 << i)) && ((memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)) {
			return i; // Valid Memory type, so return its index
		}
	}

	throw std::runtime_error("Failed to find a suitable memory type!");

	return UINT32_MAX;
}
#endif