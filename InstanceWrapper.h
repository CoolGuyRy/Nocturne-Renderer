#ifndef INSTANCE_WRAPPER_H
#define INSTANCE_WRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

/*

	TODO: Add proper Validation Layer handling

*/

class InstanceWrapper {
public:
	InstanceWrapper();
	~InstanceWrapper();

	VkInstance GetInstance();
private:
	void CreateInstance();

	void OutputInstanceExtensions();
	void OutputLayers();

	bool checkInstanceExtensionSupport();
	bool checkValidationLayerSupport();

	VkInstance mInstance;
	std::vector<const char*> mExtensions;		// Instance Level Extensions List
};
#endif
