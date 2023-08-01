#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN

#include <iostream>

#include "WindowWrapper.h"
#include "Renderer.h"

int main() {
	try {
		WindowWrapper gWindow;
		Renderer gRenderer;

		while (!glfwWindowShouldClose(gWindow.GetWindow())) {
			glfwPollEvents();
		}

	} catch (const std::runtime_error& e) {
		std::cout << "Nocturne Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}