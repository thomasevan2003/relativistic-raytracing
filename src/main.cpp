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
		gui.draw(graphics_manager.width(), graphics_manager.height(), latitude, longitude, shader_program);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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