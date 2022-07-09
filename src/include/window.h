#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class WindowClass {
public:
	GLFWwindow* window;
	WindowClass(int window_width, int window_height, const char* window_title)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
		if (!window) {
			std::cerr << "Failed to create GLFW window\n" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

#endif // ! WINDOW_H