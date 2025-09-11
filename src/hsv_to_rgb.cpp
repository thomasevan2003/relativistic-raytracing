#include "hsv_to_rgb.hpp"

#include <cmath>

RGB hsv_to_rgb(HSV hsv) {
	float H = hsv.h*360.0f;
	float S = hsv.s*1.0f;
	float V = hsv.v*1.0f;
	float C = V*S;
	float H_prime = H/60.0f;
	float X = C*(1.0f - fabs(fmod(H_prime, 2.0f) - 1.0f));
	RGB rgb;
	if (0.0f <= H_prime && H_prime < 1.0f) {
		rgb = {C,X,0.0f};
	} else if (1.0f <= H_prime && H_prime < 2.0f) {
		rgb = {X,C,0.0f};
	} else if (2.0f <= H_prime && H_prime < 3.0f) {
		rgb = {0.0f,C,X};
	} else if (3.0f <= H_prime && H_prime < 4.0f) {
		rgb = {0.0f,X,C};
	} else if (4.0f <= H_prime && H_prime < 5.0f) {
		rgb = {X,0.0f,C};
	} else if (5.0f <= H_prime && H_prime < 6.0f) {
		rgb = {C,0.0f,X};
	}
	float m = V - C;
	rgb.r += m;
	rgb.g += m;
	rgb.b += m;
	return rgb;
}
