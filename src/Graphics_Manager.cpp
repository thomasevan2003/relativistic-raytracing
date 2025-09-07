#include "Graphics_Manager.hpp"

#include <glad/gl.h>
#include <cstdlib>
#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "build_settings.hpp"

void Graphics_Manager::initialize() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		std::exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WINDOW_WIDTH_START, WINDOW_HEIGHT_START, "RelativisticRaytracer", NULL, NULL);
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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void Graphics_Manager::terminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Graphics_Manager::window_open() {
	return !glfwWindowShouldClose(window);
}

void Graphics_Manager::start_frame() {
	glfwPollEvents();
	glfwGetFramebufferSize(window, &m_width, &m_height);
	m_left = false;
	m_right = false;
	m_up = false;
	m_down = false;
	int state = glfwGetKey(window, GLFW_KEY_LEFT);
	if (state == GLFW_PRESS) {
		m_left = true;
	}
	state = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (state == GLFW_PRESS) {
		m_right = true;
	}
	state = glfwGetKey(window, GLFW_KEY_UP);
	if (state == GLFW_PRESS) {
		m_up = true;
	}
	state = glfwGetKey(window, GLFW_KEY_DOWN);
	if (state == GLFW_PRESS) {
		m_down = true;
	}
}

void Graphics_Manager::end_frame() {
	glfwSwapBuffers(window);
}

int Graphics_Manager::width() {
	return m_width;
}
int Graphics_Manager::height() {
	return m_height;
}

bool Graphics_Manager::key_left() {
	return m_left;
}
bool Graphics_Manager::key_right() {
	return m_right;
}
bool Graphics_Manager::key_up() {
	return m_up;
}
bool Graphics_Manager::key_down() {
	return m_down;
}