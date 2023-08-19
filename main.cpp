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
		// std::string justFileName = filename.substr(0, filename.rfind('.'));

		if (extension != ".spv") {
			std::cout << "Precompiling "; std::string command = "C:\\VulkanSDK\\1.3.250.1\\Bin\\glslangValidator.exe -o .\\Resources\\Shaders\\" + filename + ".spv -V .\\Resources\\Shaders\\" + filename;
			int returnValue = system(command.c_str());
			if (returnValue != 0) {
				throw std::runtime_error("Failed to pre-compile shader!");
			}
		} else {
			continue;
		}
	}
}

int main() {
	int gProgramSuccess = EXIT_SUCCESS;

	try {
		PreCompileShaders();

		WindowWrapper gWindow;
		Renderer gRenderer(&gWindow);

		float angle = 0.0f;
		float deltaTime = 0.0f;
		float lastTime = 0.0f;

		while (!glfwWindowShouldClose(gWindow.GetWindow())) {
			float now = (float)glfwGetTime();
			deltaTime = now - lastTime;
			lastTime = now;

			glfwPollEvents();

			angle = angle + 1.0f * deltaTime;

			glm::mat4 model1(1.0f);

			model1 = glm::scale(model1, glm::vec3(0.5f, 0.5f, 0.5f));

			model1 = glm::translate(model1, glm::vec3(-2.0f, 0.0f, 0.0f));

			model1 = glm::rotate(model1, angle, glm::vec3(1.0f, 0.0f, 1.0f));			

			gRenderer.UpdateModel(0, model1);

			glm::mat4 model2(1.0f);

			model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 0.5f));

			model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f));

			model2 = glm::rotate(model2, -angle, glm::vec3(1.0f, 0.0f, 1.0f));

			gRenderer.UpdateModel(1, model2);

			gRenderer.Draw();
		}

	} catch (const std::runtime_error& e) {
		std::cout << "Nocturne Error: " << e.what() << std::endl;
		gProgramSuccess = EXIT_FAILURE;
	}

	#ifndef NDEBUG
		system("PAUSE");
	#endif

	return gProgramSuccess;
}