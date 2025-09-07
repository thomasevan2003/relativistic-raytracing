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
	float t = 0.0;
	float x = -20.0*d_camera.x;
	float y = -20.0*d_camera.y;
	float z = -20.0*d_camera.z;
	float r = sqrt(x*x+y*y+z*z);
	float theta = acos(z/r);
	float phi = atan(y,x);
	float tdot = length(d);
	float xdot = d.x;
	float ydot = d.y;
	float zdot = d.z;
	float rdot = (x*xdot+y*ydot+z*zdot)/r;
	float thetadot = -(x*x*zdot-x*z*xdot+y*y*zdot-y*z*ydot)/(r*r*sqrt(x*x+y*y));
	float phidot = (x*ydot-y*xdot)/(x*x+y*y);
	// integration loop here
	d.xyz = vec3(sin(theta)*cos(phi)*rdot + r*cos(theta)*cos(phi)*thetadot - r*sin(theta)*sin(phi)*phidot, 
	             sin(theta)*sin(phi)*rdot + r*cos(theta)*sin(phi)*thetadot + r*sin(theta)*cos(phi)*phidot,
				 cos(theta)*rdot - r*sin(theta)*thetadot);
	float theta_starmap = asin(d.y);
	float phi_starmap = atan(d.x, -d.z);	
	FragColor = texture(starmap, vec2(phi_starmap/(2.0*3.14159265) + 0.5, theta_starmap/3.14159265 + 0.5));
}