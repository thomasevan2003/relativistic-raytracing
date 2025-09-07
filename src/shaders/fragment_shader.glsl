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
	vec2 screen_coords = vec2(2.0*(gl_FragCoord.x-viewportX)/viewportWidth - 1.0, 1.0 - 2.0*gl_FragCoord.y/viewportHeight); // screen space coords from -1.0 to 1.0
	float aspect = viewportWidth / viewportHeight;
	float scaleHeight = tan(fovHeight * 0.5); // scaling factor based on field of view: height of point (0,1) in screen space divided by distance from camera to screen
	float screen_y = screen_coords.y*scaleHeight;
	float screen_x = screen_coords.x*aspect*scaleHeight;
	vec3 d = normalize(vec3(screen_x,screen_y,-1.0)); // direction vector for given pixel
	mat3 rotX = mat3(1,0,0, 
					 0,cos(latitude),-sin(latitude), 
					 0,sin(latitude),cos(latitude));
	mat3 rotY = mat3(cos(longitude),0,-sin(longitude), 
					 0,1,0, 
					 sin(longitude),0,cos(longitude));
	d = rotY*rotX*d;
	vec3 d_camera = rotY*rotX*vec3(0.0,0.0,-1.0);
	vec3 x_cartesian = vec3(-20.0*d_camera); // starting position at time 0, facing the black hole
	float r = length(x_cartesian);
	vec4 x = vec4(0.0, r, acos(x_cartesian.z/r), atan(x_cartesian.y,x_cartesian.x));
	vec4 dx_dlambda = vec4(length(d),d);
	// integration loop here
	d = dx_dlambda.yzw;
	float theta_starmap = asin(d.y);
	float phi_starmap = atan(d.x, -d.z);	
	FragColor = texture(starmap, vec2(phi_starmap/(2.0*3.14159265) + 0.5, theta_starmap/3.14159265 + 0.5));
}