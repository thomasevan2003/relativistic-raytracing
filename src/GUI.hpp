#ifndef GUI_HPP
#define GUI_HPP

class GUI {
	public:
		
		void draw(int width, int height);
		int viewport_width();
		int viewport_height();
		
	private:
	
		int m_viewport_width;
		int m_viewport_height;
	
};

#endif