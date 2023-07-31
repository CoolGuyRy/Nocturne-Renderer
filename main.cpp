#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <GLFW/glfw3.h>

GLFWwindow* gWindow;

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	gWindow = glfwCreateWindow(800, 600, "Nocturne Renderer", nullptr, nullptr);

	while (!glfwWindowShouldClose(gWindow)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(gWindow);

	glfwTerminate();

	return 0;
}