#ifndef GUI_HPP
#define GUI_HPP

class GUI {
	public:
		
		GUI();
		void draw(int width, int height);
		int viewport_width();
		int viewport_height();
		int viewport_x();
		float fov();
		
	private:
	
		int m_viewport_width;
		int m_viewport_height;
		float m_fov;
	
};

#endif