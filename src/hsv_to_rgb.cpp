#include "hsv_to_rgb.hpp"

#include <cmath>

RGB hsv_to_rgb(HSV hsv) {
	float H = hsv.h*360.0;
	float S = hsv.s*1.0;
	float V = hsv.v*1.0;
	float C = V*S;
	float H_prime = H/60.0;
	float X = C*(1.0 - fabs(fmod(H_prime, 2.0) - 1.0);
	RGB rgb;
	if (0.0 <= H_prime < 1.0) {
		rgb = {C,X,0.0};
	} else if (1.0 <= H_prime < 2.0) {
		rgb = {X,C,0.0};
	} else if (2.0 <= H_prime < 3.0) {
		rgb = {0.0,C,X};
	} else if (3.0 <= H_prime < 4.0) {
		rgb = {0.0,X,C};
	} else if (4.0 <= H_prime < 5.0) {
		rgb = {X,0.0,C};
	} else if (5.0 <= H_prime < 6.0) {
		rgb = {C,0.0,X};
	}
}
