#ifndef HSV_TO_RGB
#define HSV_TO_RGB

#include <vector>

struct RGB {
	float r;
	float g;
	float b;
};

struct HSV {
	float h;
	float s;
	float v;
};

RGB hsv_to_rgb(HSV hsv);

#endif