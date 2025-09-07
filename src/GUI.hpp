#ifndef GUI_HPP
#define GUI_HPP

class GUI {
	public:
		
		GUI();
		void draw(int width, int height);
		int viewport_width();
		int viewport_height();
		int viewport_x();
		void set_fps(double fps);
		float fov();
		
	private:
	
		int m_viewport_width;
		int m_viewport_height;
		float m_fov;
		double m_fps;
		double m_last_fps_time;
		int m_fps_frames;
		bool m_vsync;
	
};

#endif