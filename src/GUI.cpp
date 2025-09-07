#include "GUI.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "build_settings.hpp"

GUI::GUI() {
	m_fov = FOV_HEIGHT_DEGREES_START;
	m_fps = 0.0;
	m_last_fps_time = glfwGetTime();
	m_fps_frames = 0;
	m_vsync = VSYNC_START;
	glfwSwapInterval(m_vsync);
}

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
	ImGui::Text("Field of View (deg)");
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH);
	ImGui::SliderFloat("##hidden", &m_fov, 0.0, 180.0);
	bool vsync_last = m_vsync;
	ImGui::Checkbox("vsync", &m_vsync);
	if (m_vsync != vsync_last) {
		glfwSwapInterval(m_vsync ? 1 : 0);
	}
	double time = glfwGetTime();
	double fps_time = time - m_last_fps_time;
	if (fps_time > FPS_REFRESH_TIME) {
		m_last_fps_time = time;
		m_fps = m_fps_frames/fps_time;
		m_fps_frames = 0;
	}
	ImGui::Text("%.1f fps", m_fps);
	ImGui::EndChild();
	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::Render();
	glViewport(CONTROL_BAR_WIDTH, 0, width-CONTROL_BAR_WIDTH, height);
	m_viewport_width = width-CONTROL_BAR_WIDTH;
	m_viewport_height = height;
	++m_fps_frames; 
}

int GUI::viewport_width() {
	return m_viewport_width;
}
int GUI::viewport_height() {
	return m_viewport_height;
}
int GUI::viewport_x() {
	return CONTROL_BAR_WIDTH;
}
float GUI::fov() {
	return m_fov;
}

void GUI::set_fps(double fps) {
	m_fps = fps;
}