#ifndef GRAPHICS_MANAGER_HPP
#define GRAPHICS_MANAGER_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Graphics_Manager {
	private:
		GLFWwindow* window;
		int m_width;
		int m_height;
		bool m_left;
		bool m_right;
		bool m_up;
		bool m_down;
	public:
		void initialize();
		void terminate();
		bool window_open();
		void start_frame();
		void end_frame();
		int width();
		int height();
		bool key_up();
		bool key_left();
		bool key_right();
		bool key_down();
};

#endif