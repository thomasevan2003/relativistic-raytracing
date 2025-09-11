#include "GUI.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "build_settings.hpp"
#include <cmath>

GUI::GUI() {
	fov = FOV_HEIGHT_DEGREES_START;
	fps = 0.0;
	R_s = 1.0;
	last_fps_time = glfwGetTime();
	fps_frames = 0;
	r_camera = INITIAL_R_CAMERA;
	log10_timestep_scale = INITIAL_LOG10_TIMESTEP_SCALE;
	maxsteps = INITIAL_MAXSTEPS;
	vsync = VSYNC_START;
	show_accretion_disk = SHOW_ACCRETION_DISK_START;
	glfwSwapInterval(vsync);
}

void GUI::draw(int width, int height, double latitude, double longitude, unsigned int shader_program) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2(0, (float)height));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(UI_PADDING,UI_PADDING));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2,2));
	ImGui::Begin("Main Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
									  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollWithMouse | 
									  ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));		
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
	ImGui::BeginChild("Controls", ImVec2(CONTROL_BAR_WIDTH-2*UI_PADDING,0), true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
																	 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | 
																	 ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Field of View (deg)");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::SliderFloat("##0", &fov, 0.0, 180.0);
	ImGui::PopStyleVar(1);
	ImGui::Text("R_s");
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::SliderFloat("##1", &R_s, 0.0, MAX_R_S);
	ImGui::PopStyleVar(1);
	ImGui::Text("Distance (x R_s)");
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::SliderFloat("##2", &r_camera, 0.0, MAX_R_CAMERA);
	ImGui::PopStyleVar(1);
	ImGui::Text("log10(timestep_scale)");
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::SliderFloat("##3", &log10_timestep_scale, MIN_LOG10_TIMESTEP_SCALE, MAX_LOG10_TIMESTEP_SCALE);
	ImGui::PopStyleVar(1);
	ImGui::Text("Max steps per photon");
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::SliderInt("##4", &maxsteps, 0, MAX_MAXSTEPS);
	ImGui::PopStyleVar(1);
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::Checkbox("Accretion Disk", &show_accretion_disk);
	ImGui::PopStyleVar(1);
	bool vsync_last = vsync;
	ImGui::SetNextItemWidth(CONTROL_BAR_WIDTH-2*UI_PADDING);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::Checkbox("vsync", &vsync);
	ImGui::PopStyleVar(1);
	if (vsync != vsync_last) {
		glfwSwapInterval(vsync ? 1 : 0);
	}
	ImGui::Text("longitude: %.1f deg", longitude*180.0/3.14159265);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::Text("latitude:  %.1f deg", latitude*180.0/3.14159265);
	ImGui::PopStyleVar(1);
	double time = glfwGetTime();
	double fps_time = time - last_fps_time;
	if (fps_time > FPS_REFRESH_TIME) {
		last_fps_time = time;
		fps = fps_frames/fps_time;
		fps_frames = 0;
	}
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,UI_ITEM_SPACING));
	ImGui::Text("%.1f fps", fps);
	ImGui::PopStyleVar(1);
	ImGui::EndChild();
	ImGui::PopStyleVar(2);
	ImGui::End();
	ImGui::PopStyleVar(2);
	ImGui::Render();
	glViewport(CONTROL_BAR_WIDTH, 0, width-CONTROL_BAR_WIDTH, height);
	viewport_width = width-CONTROL_BAR_WIDTH;
	viewport_height = height;
	glUniform1f(glGetUniformLocation(shader_program, "viewportX"), (float)CONTROL_BAR_WIDTH);
	glUniform1f(glGetUniformLocation(shader_program, "viewportWidth"), (float)viewport_width);
	glUniform1f(glGetUniformLocation(shader_program, "viewportHeight"), (float)viewport_height);
	glUniform1f(glGetUniformLocation(shader_program, "fovHeight"), (float)(fov*3.14159265/180.0));
	glUniform1f(glGetUniformLocation(shader_program, "latitude"), (float)latitude);
	glUniform1f(glGetUniformLocation(shader_program, "longitude"), (float)longitude);
	glUniform1f(glGetUniformLocation(shader_program, "R_s"), (float)R_s);
	glUniform1f(glGetUniformLocation(shader_program, "r_camera"), (float)r_camera);
	glUniform1f(glGetUniformLocation(shader_program, "timestep_scale"), pow(10.0f, (float)log10_timestep_scale));
	glUniform1i(glGetUniformLocation(shader_program, "maxsteps"), (int)maxsteps);
	glUniform1f(glGetUniformLocation(shader_program, "time"), (float)(glfwGetTime()+777.0));
	glUniform1i(glGetUniformLocation(shader_program, "show_accretion_disk"), (int)show_accretion_disk);
	++fps_frames; 
}

