#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "InstanceWrapper.h"
#include "PhysicalDeviceWrapper.h"

class Renderer {
public:
	Renderer();
	~Renderer();
private:
	InstanceWrapper* mInstance;
	PhysicalDeviceWrapper* mPhysicalDevice;
};

#endif