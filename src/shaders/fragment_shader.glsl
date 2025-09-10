#version 400 core
out vec4 FragColor;
uniform float viewportX;
uniform float viewportWidth;
uniform float viewportHeight;
uniform float fovHeight;
uniform float latitude;
uniform float longitude;
uniform float R_s;
uniform float r_camera;
uniform float timestep_scale;
uniform int maxsteps;
uniform sampler2D starmap;

struct State {
	float t;
	float r;
	float theta;
	float phi;
	float tdot;
	float rdot;
	float thetadot;
	float phidot;
};

State f(State X) {
	State Xdot;
	Xdot.t = X.tdot;
	Xdot.r = X.rdot;
	Xdot.theta = X.thetadot;
	Xdot.phi = X.phidot;
	Xdot.tdot = R_s/(X.r*(R_s - X.r))*X.tdot*X.rdot;
	Xdot.rdot = R_s*(R_s-X.r)/(2*X.r*X.r*X.r)*X.tdot*X.tdot - R_s/(2*X.r*(R_s-X.r))*X.rdot*X.rdot - (R_s-X.r)*X.thetadot*X.thetadot - sin(X.theta)*sin(X.theta)*(R_s-X.r)*X.phidot*X.phidot;
	Xdot.thetadot = -2/X.r*X.rdot*X.thetadot + sin(2*X.theta)/2*X.phidot*X.phidot;
	Xdot.phidot = -2/X.r*X.rdot*X.phidot - 2*cos(X.theta)/sin(X.theta)*X.thetadot*X.phidot;
	return Xdot;
}

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
	State X;
	X.t = t;
	X.r = r;
	X.theta = theta;
	X.phi = phi;
	X.tdot = tdot;
	X.rdot = rdot;
	X.thetadot = thetadot;
	X.phidot = phidot;
	bool captured_by_event_horizon = false;
	int steps = 0;
	if (R_s == 0.0) {
		steps = maxsteps;
	}
	while (steps < maxsteps) {
		++steps;
		
		float r_r = X.r/R_s;
		float dlambda = timestep_scale*r_r*sqrt(r_r)*sin(X.theta)*R_s;
		
		State Xdot = f(X);
		X.t += Xdot.t*dlambda;
		X.r += Xdot.r*dlambda;
		X.theta += Xdot.theta*dlambda;
		X.phi += Xdot.phi*dlambda;
		X.tdot += Xdot.tdot*dlambda;
		X.rdot += Xdot.rdot*dlambda;
		X.thetadot += Xdot.thetadot*dlambda;
		X.phidot += Xdot.phidot*dlambda;
		
		/*State X1 = X;
		State K1 = f(X1);
		State X2;
		X2.t = X.t + dlambda*K1.t/2.0;
		X2.r = X.r + dlambda*K1.t/2.0;
		X2.theta = X.theta + dlambda*K1.t/2.0;
		X2.phi = X.phi + dlambda*K1.t/2.0;
		X2.tdot = X.tdot + dlambda*K1.tdot/2.0;
		X2.rdot = X.rdot + dlambda*K1.rdot/2.0;
		X2.thetadot = X.thetadot + dlambda*K1.thetadot/2.0;
		X2.phidot = X.phidot + dlambda*K1.phidot/2.0;
		State K2 = f(X2);
		State X3;
		X3.t = X.t + dlambda*K2.t/2.0;
		X3.r = X.r + dlambda*K2.t/2.0;
		X3.theta = X.theta + dlambda*K2.t/2.0;
		X3.phi = X.phi + dlambda*K2.t/2.0;
		X3.tdot = X.tdot + dlambda*K2.tdot/2.0;
		X3.rdot = X.rdot + dlambda*K2.rdot/2.0;
		X3.thetadot = X.thetadot + dlambda*K2.thetadot/2.0;
		X3.phidot = X.phidot + dlambda*K2.phidot/2.0;
		State K3 = f(X3);
		State X4;
		X4.t = X.t + dlambda*K3.t;
		X4.r = X.r + dlambda*K3.t;
		X4.theta = X.theta + dlambda*K3.t;
		X4.phi = X.phi + dlambda*K3.t;
		X4.tdot = X.tdot + dlambda*K3.tdot;
		X4.rdot = X.rdot + dlambda*K3.rdot;
		X4.thetadot = X.thetadot + dlambda*K3.thetadot;
		X4.phidot = X.phidot + dlambda*K3.phidot;
		State K4 = f(X4);
		X.t = X.t + dlambda/6.0*(K1.t + 2.0*K2.t + 2.0*K3.t + K4.t);
		X.r = X.r + dlambda/6.0*(K1.r + 2.0*K2.r + 2.0*K3.r + K4.r);
		X.theta = X.theta + dlambda/6.0*(K1.theta + 2.0*K2.theta + 2.0*K3.theta + K4.theta);
		X.phi = X.phi + dlambda/6.0*(K1.phi + 2.0*K2.phi + 2.0*K3.phi + K4.phi);
		X.tdot = X.tdot + dlambda/6.0*(K1.tdot + 2.0*K2.tdot + 2.0*K3.tdot + K4.tdot);
		X.rdot = X.rdot + dlambda/6.0*(K1.rdot + 2.0*K2.rdot + 2.0*K3.rdot + K4.rdot);
		X.thetadot = X.thetadot + dlambda/6.0*(K1.thetadot + 2.0*K2.thetadot + 2.0*K3.thetadot + K4.thetadot);
		X.phidot = X.phidot + dlambda/6.0*(K1.phidot + 2.0*K2.phidot + 2.0*K3.phidot + K4.phidot);*/
		
		if (X.r < R_s) {
			captured_by_event_horizon = true;
			break;
		}
		if (X.r > r_camera && X.rdot > 0.0) {
			break;
		}
	}
	vec3 d_final = vec3(sin(X.theta)*cos(X.phi)*X.rdot + X.r*cos(X.theta)*cos(X.phi)*X.thetadot - X.r*sin(X.theta)*sin(X.phi)*X.phidot, 
	                    sin(X.theta)*sin(X.phi)*X.rdot + X.r*cos(X.theta)*sin(X.phi)*X.thetadot + X.r*sin(X.theta)*cos(X.phi)*X.phidot,
				        cos(X.theta)*X.rdot - X.r*sin(X.theta)*X.thetadot);
	float theta_starmap = asin(d_final.y);
	float phi_starmap = atan(d_final.x, -d_final.z);	
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	if (!captured_by_event_horizon) {
		FragColor = texture(starmap, vec2(phi_starmap/(2.0*3.14159265) + 0.5, theta_starmap/3.14159265 + 0.5));
	}
	//FragColor = vec4(float(steps)/float(maxsteps), float(steps)/float(maxsteps), float(steps)/float(maxsteps), 1.0);  
	//FragColor = vec4(thetamin/3.1415, thetamin/3.1415, thetamin/3.1415, 1.0);
}