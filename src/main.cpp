#include <iostream>
#include <imgui.h>
#include "Graphics_Manager.hpp"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "build_settings.hpp"
#include "file_to_string.hpp"
#include "stb_image.h"

void run() {
	constexpr float vertices[] = { -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f };
	double latitude = 0.0;
	double longitude = 0.0;
	int texture_width, texture_height, nrChannels;
	unsigned char *texture_data = stbi_load(STARMAP_PATH, &texture_width, &texture_height, &nrChannels, 0); 
	Graphics_Manager graphics_manager; 
	graphics_manager.initialize();
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertex_shader_source = file_to_string(VERTEX_SHADER_PATH);
	const char* vertex_shader_ptr = vertex_shader_source.c_str();
	glShaderSource(vertex_shader, 1, &vertex_shader_ptr, NULL);
	glCompileShader(vertex_shader);
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cerr << "Vertex shader failed to compile\n" << info_log << std::endl;
		std::exit(-1);
	}
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragment_shader_source = file_to_string(FRAGMENT_SHADER_PATH);
	const char* fragment_shader_ptr = fragment_shader_source.c_str();
	glShaderSource(fragment_shader, 1, &fragment_shader_ptr, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cerr << "Fragment shader failed to compile\n" << info_log << std::endl;
		std::exit(-1);
	}
	unsigned int shader_program;
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		std::cerr << "Shader failed to link\n" << info_log << std::endl;
		std::exit(-1);
	}
	glUseProgram(shader_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glBindVertexArray(VAO);
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, texture_width, texture_height, 0, format, GL_UNSIGNED_BYTE, texture_data);
	glUniform1i(glGetUniformLocation(shader_program, "starmap"), 0);
	stbi_image_free(texture_data);
	double last_time = -1.0;
	double time = -1.0;
	while (graphics_manager.window_open()) {
		graphics_manager.start_frame();
		last_time = time;
		time = glfwGetTime();
		double dt = time - last_time;
		if (last_time < 0.0) {
			dt = 0.0;
		}
		if (graphics_manager.key_left()) {
			longitude -= LONGITUDE_SCROLL_RATE*dt;
		}
		if (graphics_manager.key_right()) {
			longitude += LONGITUDE_SCROLL_RATE*dt;
		}
		if (graphics_manager.key_up()) {
			latitude -= LATITUDE_SCROLL_RATE*dt;
		}
		if (graphics_manager.key_down()) {
			latitude += LATITUDE_SCROLL_RATE*dt;
		}
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(0,0));
		ImGui::SetNextWindowSize(ImVec2(0, (float)graphics_manager.height()));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
		ImGui::Begin("Main Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
										  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollWithMouse | 
										  ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse);
		ImGui::BeginChild("Controls", ImVec2(CONTROL_BAR_WIDTH,0), true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
																		 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | 
																		 ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse);
		ImGui::EndChild();
		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::Render();
		glViewport(CONTROL_BAR_WIDTH, 0, graphics_manager.width()-CONTROL_BAR_WIDTH, graphics_manager.height());
		glUniform1f(glGetUniformLocation(shader_program, "viewportWidth"), (float)(graphics_manager.width()-CONTROL_BAR_WIDTH));
		glUniform1f(glGetUniformLocation(shader_program, "viewportHeight"), (float)(graphics_manager.height()));
		glUniform1f(glGetUniformLocation(shader_program, "fovHeight"), (float)(FOV_HEIGHT_DEGREES*3.14159265/180.0));
		glUniform1f(glGetUniformLocation(shader_program, "latitude"), (float)latitude);
		glUniform1f(glGetUniformLocation(shader_program, "longitude"), (float)longitude);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		graphics_manager.end_frame();
	}
	graphics_manager.terminate();
}

int main() {
	run();
	std::cout << "END OF PROGRAM" << std::endl;
}