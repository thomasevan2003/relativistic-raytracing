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
		unsigned int m_framebuffer;
		unsigned int m_framebuffer_texture;
		int m_framebuffer_width;
		int m_framebuffer_height;
	public:
		void initialize();
		void terminate();
		bool window_open();
		void start_frame(float downsample_rate);
		void end_frame(float downsample_rate);
		int width();
		int height();
		bool key_up();
		bool key_left();
		bool key_right();
		bool key_down();
};

#endif