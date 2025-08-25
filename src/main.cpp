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

void run() {
	double latitude = 0.0;
	double longitude = 0.0;
	Graphics_Manager graphics_manager; 
	graphics_manager.initialize();
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