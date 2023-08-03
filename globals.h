#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

const uint32_t MAX_CONCURRENT_FRAMES = 3;

const std::vector<const char*> ENABLED_VALIDATION_LAYERS = {
	"VK_LAYER_KHRONOS_validation",
	//"VK_LAYER_LUNARG_api_dump",
	//"VK_LAYER_LUNARG_gfxreconstruct",
	"VK_LAYER_KHRONOS_synchronization2",
};
const std::vector<const char*> ENABLED_INSTANCE_EXTENSIONS = { 
	
};
const std::vector<const char*> ENABLED_PHYSICAL_DEVICE_EXTENSIONS = { 

};
const std::vector<const char*> ENABLED_LOGICAL_DEVICE_EXTENSIONS = { 
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const VkPhysicalDeviceFeatures ENABLED_PHYSICAL_DEVICE_FEATURES = {
	0, // VkBool32    robustBufferAccess;
	0, // VkBool32    fullDrawIndexUint32;
	0, // VkBool32    imageCubeArray;
	0, // VkBool32    independentBlend;
	0, // VkBool32    geometryShader;
	0, // VkBool32    tessellationShader;
	0, // VkBool32    sampleRateShading;
	0, // VkBool32    dualSrcBlend;
	0, // VkBool32    logicOp;
	0, // VkBool32    multiDrawIndirect;
	0, // VkBool32    drawIndirectFirstInstance;
	0, // VkBool32    depthClamp;
	0, // VkBool32    depthBiasClamp;
	0, // VkBool32    fillModeNonSolid;
	0, // VkBool32    depthBounds;
	0, // VkBool32    wideLines;
	0, // VkBool32    largePoints;
	0, // VkBool32    alphaToOne;
	0, // VkBool32    multiViewport;
	0, // VkBool32    samplerAnisotropy;
	0, // VkBool32    textureCompressionETC2;
	0, // VkBool32    textureCompressionASTC_LDR;
	0, // VkBool32    textureCompressionBC;
	0, // VkBool32    occlusionQueryPrecise;
	0, // VkBool32    pipelineStatisticsQuery;
	0, // VkBool32    vertexPipelineStoresAndAtomics;
	0, // VkBool32    fragmentStoresAndAtomics;
	0, // VkBool32    shaderTessellationAndGeometryPointSize;
	0, // VkBool32    shaderImageGatherExtended;
	0, // VkBool32    shaderStorageImageExtendedFormats;
	0, // VkBool32    shaderStorageImageMultisample;
	0, // VkBool32    shaderStorageImageReadWithoutFormat;
	0, // VkBool32    shaderStorageImageWriteWithoutFormat;
	0, // VkBool32    shaderUniformBufferArrayDynamicIndexing;
	0, // VkBool32    shaderSampledImageArrayDynamicIndexing;
	0, // VkBool32    shaderStorageBufferArrayDynamicIndexing;
	0, // VkBool32    shaderStorageImageArrayDynamicIndexing;
	0, // VkBool32    shaderClipDistance;
	0, // VkBool32    shaderCullDistance;
	0, // VkBool32    shaderFloat64;
	0, // VkBool32    shaderInt64;
	0, // VkBool32    shaderInt16;
	0, // VkBool32    shaderResourceResidency;
	0, // VkBool32    shaderResourceMinLod;
	0, // VkBool32    sparseBinding;
	0, // VkBool32    sparseResidencyBuffer;
	0, // VkBool32    sparseResidencyImage2D;
	0, // VkBool32    sparseResidencyImage3D;
	0, // VkBool32    sparseResidency2Samples;
	0, // VkBool32    sparseResidency4Samples;
	0, // VkBool32    sparseResidency8Samples;
	0, // VkBool32    sparseResidency16Samples;
	0, // VkBool32    sparseResidencyAliased;
	0, // VkBool32    variableMultisampleRate;
	0  // VkBool32    inheritedQueries;
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
#endif