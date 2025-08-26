#version 330 core
out vec4 FragColor;
uniform float viewportX;
uniform float viewportWidth;
uniform float viewportHeight;
uniform float fovHeight;
uniform float latitude;
uniform float longitude;
uniform sampler2D starmap;
void main() {
	vec2 coords = vec2(2.0*(gl_FragCoord.x-viewportX)/viewportWidth - 1.0, 1.0 - 2.0*gl_FragCoord.y/viewportHeight); // screen space coords from -1.0 to 1.0
	float aspect = viewportWidth / viewportHeight;
	float scaleHeight = tan(fovHeight * 0.5); // scaling factor based on field of view: height of point (0,1) in screen space divided by distance from camera to screen
	float y = coords.y*scaleHeight;
	float x = coords.x*aspect*scaleHeight;
	vec3 ray = normalize(vec3(x,y,-1.0)); // direction vector for given pixel
	mat3 rotX = mat3(1,0,0, 
					 0,cos(latitude),-sin(latitude), 
					 0,sin(latitude),cos(latitude));
	mat3 rotY = mat3(cos(longitude),0,-sin(longitude), 
					 0,1,0, 
					 sin(longitude),0,cos(longitude));
	ray = rotY*rotX*ray;
	float theta = asin(ray.y);
	float phi = atan(ray.x, -ray.z);
	FragColor = texture(starmap, vec2(phi/(2.0*3.14159265) + 0.5, theta/3.14159265 + 0.5));
	if (x*x + y*y < 0.01) {
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}