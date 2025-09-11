#version 400 core
out vec4 FragColor;
uniform float viewportWidth;
uniform float viewportHeight;
uniform float fovHeight;
uniform float latitude;
uniform float longitude;
uniform float R_s;
uniform float r_camera;
uniform float timestep_scale;
uniform float time;
uniform int maxsteps;
uniform int show_accretion_disk;
uniform float accretion_disk_size;
uniform sampler2D starmap;
uniform sampler2D disk_ring;

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
	vec2 screen_coords = vec2(2.0*gl_FragCoord.x/viewportWidth - 1.0, 1.0 - 2.0*gl_FragCoord.y/viewportHeight); // screen space coords from -1.0 to 1.0
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
	vec4 disk_colors[5];
	vec3 disk_emission[5];
	int n_disk_passes = 0;
	while (steps < maxsteps) {
		++steps;
		
		float r_r = X.r/R_s;
		float dlambda = timestep_scale*r_r*sqrt(r_r)*sin(X.theta)*R_s;
		
		float start_phi = X.phi;
		
		State Xdot = f(X);
		X.t += Xdot.t*dlambda;
		X.r += Xdot.r*dlambda;
		X.theta += Xdot.theta*dlambda;
		X.phi += Xdot.phi*dlambda;
		X.tdot += Xdot.tdot*dlambda;
		X.rdot += Xdot.rdot*dlambda;
		X.thetadot += Xdot.thetadot*dlambda;
		X.phidot += Xdot.phidot*dlambda;
		
		// if path crosses equator, check for collision with accretion disk
		if (bool(show_accretion_disk) && (sin(start_phi))*(sin(X.phi)) < 0.0 && n_disk_passes < 2 && r_r < accretion_disk_size && r_r > 3.0) {
			float r_fraction = (r_r - 3.0)/(accretion_disk_size - 3.0);
			float disk_theta = X.theta;
			if (abs(X.phi) < 3.14/2.0) {
				disk_theta = 2.0*3.14159265 - disk_theta;
			}
			float n_rings = 3.0*accretion_disk_size;
			float ring_level = float(int(((r_r+2.5)/(accretion_disk_size-3.0)*n_rings)));
			float r_r_ring1 = ring_level*(accretion_disk_size-3.0)/n_rings;
			float r_r_ring2 = (ring_level+0.5)*(accretion_disk_size-3.0)/n_rings;
			float frequency1 = 1.0/sqrt(r_r_ring1*r_r_ring1*r_r_ring1/27.0);
			float frequency2 = 1.0/sqrt(r_r_ring2*r_r_ring2*r_r_ring2/27.0);
			float r_density_factor = pow(r_fraction,0.6)*pow(1.0-r_fraction,1.0)/0.345;
			float disk_density = r_density_factor *
			                     (0.2 + 0.4*texture(disk_ring, vec2(disk_theta/(3.14159265*2.0) + time*frequency1, r_fraction*n_rings)).x*0.7
									  + 0.4*texture(disk_ring, vec2(disk_theta/(3.14159265*2.0) + time*frequency2, r_fraction*n_rings+0.5)).x*0.7);
			disk_colors[n_disk_passes] = vec4(1.0, 0.2, 0.1, 1.0*min(disk_density*4.0,1.0));
			disk_emission[n_disk_passes] = vec3(1.0, 0.2, 0.1)*(disk_density)*14.0;
			++n_disk_passes;
		}
		// if path crosses event horizon, exit
		if (r_r < 1.0) {
			captured_by_event_horizon = true;
			break;
		}
		// if path reaches original distance traveling away, assume path is escaped
		if (X.r > r_camera && X.rdot > 0.0) {
			break;
		}
	}
	vec3 d_final = vec3(sin(X.theta)*cos(X.phi)*X.rdot + X.r*cos(X.theta)*cos(X.phi)*X.thetadot - X.r*sin(X.theta)*sin(X.phi)*X.phidot, 
	                    sin(X.theta)*sin(X.phi)*X.rdot + X.r*cos(X.theta)*sin(X.phi)*X.thetadot + X.r*sin(X.theta)*cos(X.phi)*X.phidot,
				        cos(X.theta)*X.rdot - X.r*sin(X.theta)*X.thetadot);
	float theta_starmap = asin(d_final.y);
	float phi_starmap = atan(d_final.x, -d_final.z);	
	vec4 background = vec4(0.0, 0.0, 0.0, 1.0);
	if (!captured_by_event_horizon) {
		background = texture(starmap, vec2(phi_starmap/(2.0*3.14159265) + 0.5, theta_starmap/3.14159265 + 0.5));
	}
	vec3 background_filtered = background.xyz;
	for (int i = 0; i < n_disk_passes; ++i) {
		background_filtered *= (1.0 - disk_colors[i].w);
	}
	vec3 disks_filtered = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < n_disk_passes; ++i) {
		vec3 disk_filtered = disk_colors[i].xyz*disk_colors[i].w + disk_emission[i].xyz;
		for (int j = 0; j < i; ++j) {
			disk_filtered *= (1.0 - disk_colors[j].w);
		}
		disks_filtered += disk_filtered;
	}
	FragColor.xyz = background_filtered + min(disks_filtered,1.0);
	//FragColor = vec4(float(steps)/float(maxsteps), float(steps)/float(maxsteps), float(steps)/float(maxsteps), 1.0); // show number of steps per pixel (to identify problem areas)
}