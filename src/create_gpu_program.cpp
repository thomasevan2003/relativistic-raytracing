#include "create_gpu_program.hpp"

#include "build_settings.hpp"
#include "stb_image.h"
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "file_to_string.hpp"
#include <iostream>

unsigned int create_gpu_program() {
	constexpr float vertices[] = { -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f }; // two triangles to fill viewport
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
	int starmap_texture_width, starmap_texture_height, starmap_nrChannels;
	unsigned char *starmap_texture_data = stbi_load(STARMAP_PATH, &starmap_texture_width, &starmap_texture_height, &starmap_nrChannels, 0); 
	unsigned int starmap_texture;
	glGenTextures(1, &starmap_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, starmap_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLenum starmap_format = (starmap_nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, starmap_format, starmap_texture_width, starmap_texture_height, 0, starmap_format, GL_UNSIGNED_BYTE, starmap_texture_data);
	stbi_image_free(starmap_texture_data);
	glUniform1i(glGetUniformLocation(shader_program, "starmap"), 0);
	int disk_texture_width, disk_texture_height, disk_nrChannels;
	unsigned char *disk_texture_data = stbi_load(DISK_RING_PATH, &disk_texture_width, &disk_texture_height, &disk_nrChannels, 0); 
	unsigned int disk_texture;
	glGenTextures(1, &disk_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, disk_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLenum disk_format = (disk_nrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, disk_format, disk_texture_width, disk_texture_height, 0, disk_format, GL_UNSIGNED_BYTE, disk_texture_data);
	stbi_image_free(disk_texture_data);
	glUniform1i(glGetUniformLocation(shader_program, "disk_ring"), 1);
	return shader_program;
}