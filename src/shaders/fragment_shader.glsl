#version 330 core
out vec4 FragColor;
uniform float viewportX;
uniform float viewportWidth;
uniform float viewportHeight;
uniform float fovHeight;
uniform float latitude;
uniform float longitude;
uniform float R_s;
uniform float r_camera;
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
	float x = -r_camera*d_camera.x;
	float y = -r_camera*d_camera.y;
	float z = -r_camera*d_camera.z;
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
	bool captured_by_event_horizon = false;
	for (int i = 0; i < 400; ++i) {
		float dlambda = 0.01*r;
		t += tdot*dlambda;
		r += rdot*dlambda;
		if (r < R_s) {
			captured_by_event_horizon = true;
			break;
		}
		theta += thetadot*dlambda;
		phi += phidot*dlambda;
		tdot += (R_s/(r*(R_s - r))*tdot*rdot)*dlambda;
		rdot += (R_s*(R_s-r)/(2*r*r*r)*tdot*tdot - R_s/(2*r*(R_s-r))*rdot*rdot - (R_s-r)*thetadot*thetadot - sin(theta)*sin(theta)*(R_s-r)*phidot*phidot)*dlambda;
		thetadot += (-2/r*rdot*thetadot + sin(2*theta)/2*phidot*phidot)*dlambda;
		phidot += (-2/r*rdot*phidot - 2*cos(theta)/sin(theta)*thetadot*phidot)*dlambda;
		//if (r > r_camera && rdot > 0.0) {
		//	break;
		//}
	}
	vec3 d_final = vec3(sin(theta)*cos(phi)*rdot + r*cos(theta)*cos(phi)*thetadot - r*sin(theta)*sin(phi)*phidot, 
	                    sin(theta)*sin(phi)*rdot + r*cos(theta)*sin(phi)*thetadot + r*sin(theta)*cos(phi)*phidot,
				        cos(theta)*rdot - r*sin(theta)*thetadot);
	float theta_starmap = asin(d_final.y);
	float phi_starmap = atan(d_final.x, -d_final.z);	
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	if (!captured_by_event_horizon) {
		FragColor = texture(starmap, vec2(phi_starmap/(2.0*3.14159265) + 0.5, theta_starmap/3.14159265 + 0.5));
	}
}