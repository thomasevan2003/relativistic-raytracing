#include <iostream>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Graphics_Manager.hpp"

void run() {
	Graphics_Manager graphics_manager; 
	graphics_manager.initialize();
	while (graphics_manager.window_open()) {
		graphics_manager.start_frame();
		graphics_manager.end_frame();
	}
	graphics_manager.terminate();
}

int main() {
	run();
	std::cout << "END OF PROGRAM" << std::endl;
}