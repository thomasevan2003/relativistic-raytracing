#ifndef GUI_HPP
#define GUI_HPP

class GUI {
	public:
		
		GUI();
		void draw(int width, int height, double latitude, double longitude, unsigned int shader_program);
		
	private:
	
		int viewport_width;
		int viewport_height;
		float fov;
		double fps;
		double last_fps_time;
		int fps_frames;
		bool vsync;
		float R_s;
		float r_camera;
		float log10_timestep_scale;
		int maxsteps;
		bool show_accretion_disk;
		float accretion_disk_size;
	
};

#endif