#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "WindowWrapper.h"

int main() {
	try {
		WindowWrapper gWindow;

		while (!glfwWindowShouldClose(gWindow.GetWindow())) {
			glfwPollEvents();
		}

	} catch (const std::runtime_error& e) {
		std::cout << "Nocturne Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}