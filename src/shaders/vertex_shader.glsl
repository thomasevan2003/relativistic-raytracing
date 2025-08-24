#version 330 core
layout (location = 0) in vec2 pos;
out vec3 vertexColor;
void main() {
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
	vertexColor = vec3((pos.x+1.0)/2.0, (pos.y+1.0)/2.0, (pos.y - pos.x)/2.0);
}