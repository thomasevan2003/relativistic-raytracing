#include <iostream>
#include <imgui.h>
#include "Graphics_Manager.hpp"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "build_settings.hpp"
#include "create_gpu_program.hpp"
#include "GUI.hpp"
#include <algorithm>

void run() {
	constexpr double PI = 3.14159265358979;
	double latitude = LATITUDE_START;
	double longitude = LONGITUDE_START;
	Graphics_Manager graphics_manager; 
	graphics_manager.initialize();
	GUI gui;
	unsigned int shader_program = create_gpu_program();
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
		longitude += graphics_manager.key_left() ? LONGITUDE_SCROLL_RATE*dt : 0.0;
		longitude += graphics_manager.key_right() ? -LONGITUDE_SCROLL_RATE*dt : 0.0;
		latitude += graphics_manager.key_up() ? LATITUDE_SCROLL_RATE*dt : 0.0;
		latitude += graphics_manager.key_down() ? -LATITUDE_SCROLL_RATE*dt : 0.0;
		latitude = std::clamp(latitude, -PI/2.0, PI/2.0);
		gui.draw(graphics_manager.width(), graphics_manager.height(), latitude, longitude);
		glUniform1f(glGetUniformLocation(shader_program, "viewportX"), (float)gui.viewport_x());
		glUniform1f(glGetUniformLocation(shader_program, "viewportWidth"), (float)gui.viewport_width());
		glUniform1f(glGetUniformLocation(shader_program, "viewportHeight"), (float)gui.viewport_height());
		glUniform1f(glGetUniformLocation(shader_program, "fovHeight"), (float)(gui.fov()*3.14159265/180.0));
		glUniform1f(glGetUniformLocation(shader_program, "latitude"), (float)latitude);
		glUniform1f(glGetUniformLocation(shader_program, "longitude"), (float)longitude);
		glUniform1f(glGetUniformLocation(shader_program, "R_s"), (float)gui.R_s());
		glUniform1f(glGetUniformLocation(shader_program, "r_camera"), (float)gui.r_camera());
		glUniform1f(glGetUniformLocation(shader_program, "timestep_scale"), (float)gui.timestep_scale());
		glUniform1i(glGetUniformLocation(shader_program, "maxsteps"), (int)gui.maxsteps());
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