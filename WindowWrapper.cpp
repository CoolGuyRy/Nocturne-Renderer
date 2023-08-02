#include "WindowWrapper.h"
#include "globals.h"
#include <Windows.h>

WindowWrapper::WindowWrapper() {
	if (glfwInit()) {
		std::cout << "Success: GLFW initialized." << std::endl;
	} else {
		throw std::runtime_error("Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APPLICATION_TITLE.c_str(), nullptr, nullptr);

	if (mWindow) {
		std::cout << "Success: Window created." << std::endl;
	} else {
		throw std::runtime_error("Failed to create a GLFW Window!");
	}

	RECT desktopRect;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktopRect);

	int middleX = desktopRect.right / 2 - WINDOW_WIDTH / 2;
	int middleY = desktopRect.bottom / 2 - WINDOW_HEIGHT / 2;

	glfwSetWindowPos(mWindow, middleX, middleY);
}

WindowWrapper::WindowWrapper(std::vector<std::pair<int, int>>& hints) {
	glfwInit();

	for (size_t i = 0; i < hints.size(); i++) {
		glfwWindowHint(hints[i].first, hints[i].second);
	}

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APPLICATION_TITLE.c_str(), nullptr, nullptr);
}

WindowWrapper::~WindowWrapper() {
	glfwDestroyWindow(mWindow); std::cout << "Success: Window destroyed." << std::endl;
	glfwTerminate(); std::cout << "Success: GLFW terminated." << std::endl;
}

GLFWwindow* WindowWrapper::GetWindow() {
	return mWindow;
}