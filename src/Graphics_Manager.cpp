#include "Graphics_Manager.hpp"

#include <glad/gl.h>
#include <cstdlib>
#include <iostream>

void Graphics_Manager::initialize() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(900, 600, "RelativisticRaytracer", NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create window" << std::endl;
		std::exit(-1);
	}
	glfwMakeContextCurrent(window);
	int version = gladLoadGL(glfwGetProcAddress);
	if (version == 0) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		std::exit(-1);
	}
}

void Graphics_Manager::terminate() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Graphics_Manager::window_open() {
	return !glfwWindowShouldClose(window);
}

void Graphics_Manager::start_frame() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics_Manager::end_frame() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}