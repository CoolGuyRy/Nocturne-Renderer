#include "WindowWrapper.h"

WindowWrapper::WindowWrapper() {
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APPLICATION_TITLE.c_str(), nullptr, nullptr);

	if (mWindow == nullptr) {
		throw std::runtime_error("Failed to create a GLFW Window!");
	}
}

WindowWrapper::WindowWrapper(std::vector<std::pair<int, int>>& hints) {
	glfwInit();

	for (size_t i = 0; i < hints.size(); i++) {
		glfwWindowHint(hints[i].first, hints[i].second);
	}

	mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APPLICATION_TITLE.c_str(), nullptr, nullptr);
}

WindowWrapper::~WindowWrapper() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

GLFWwindow* WindowWrapper::GetWindow() {
	return mWindow;
}
