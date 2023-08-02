#define STB_IMAGE_IMPLEMENTATION
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <filesystem>

#include "WindowWrapper.h"
#include "Renderer.h"

void PreCompileShaders() {
	std::string pathToPrint = std::filesystem::current_path().string() + "\\Resources\\Shaders\\";

	for (const auto& entry : std::filesystem::directory_iterator(pathToPrint)) {
		std::string extension = entry.path().extension().string();
		std::string filename = entry.path().filename().string();
		std::string justFileName = filename.substr(0, filename.rfind('.'));

		if (extension != ".spv") {
			std::cout << "Precompiling "; std::string command = "C:\\VulkanSDK\\1.3.250.1\\Bin\\glslangValidator.exe -o .\\Resources\\Shaders\\" + justFileName + ".spv -V .\\Resources\\Shaders\\" + filename;
			int returnValue = system(command.c_str());
		} else {
			continue;
		}
	}
}

int main() {
	PreCompileShaders();

	try {
		WindowWrapper gWindow;
		Renderer gRenderer(&gWindow);

		while (!glfwWindowShouldClose(gWindow.GetWindow())) {
			glfwPollEvents();
		}

	} catch (const std::runtime_error& e) {
		std::cout << "Nocturne Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}