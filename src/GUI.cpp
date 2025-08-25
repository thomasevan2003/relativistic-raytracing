#include "GUI.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "build_settings.hpp"

void GUI::draw(int width, int height) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2(0, (float)height));
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
	glViewport(CONTROL_BAR_WIDTH, 0, width-CONTROL_BAR_WIDTH, height);
	m_viewport_width = width-CONTROL_BAR_WIDTH;
	m_viewport_height = height;
}

int GUI::viewport_width() {
	return m_viewport_width;
}
int GUI::viewport_height() {
	return m_viewport_height;
}