#version 330 core
out vec4 FragColor;
uniform float viewportWidth;
uniform float viewportHeight;
uniform sampler2D starmap;
void main() {
	vec2 coords = vec2(gl_FragCoord.x/viewportWidth, 1.0 - gl_FragCoord.y/viewportHeight);
	//FragColor = texture(starmap, coords);
	FragColor = texture(starmap, coords);
}