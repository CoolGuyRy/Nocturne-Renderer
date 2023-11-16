#define GLM_FORCE_RADIANS

#include <iostream>
#include <filesystem>

#include "Renderer.h"

using namespace std;

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

		Renderer gRenderer;

		gRenderer.Run();

	} catch (const std::runtime_error & e) {
		std::cout << "Nocturne Error: " << e.what() << std::endl;
		gProgramSuccess = EXIT_FAILURE;
	}

	#ifndef NDEBUG
		system("PAUSE");
	#endif

	return gProgramSuccess;
}