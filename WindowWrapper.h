#ifndef WINDOW_WRAPPER_H
#define WINDOW_WRAPPER_H

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class WindowWrapper {
public:
	WindowWrapper();
	WindowWrapper(std::vector<std::pair<int, int>>&);
	~WindowWrapper();

	GLFWwindow* GetWindow();
private:
	GLFWwindow* mWindow;
};
#endif