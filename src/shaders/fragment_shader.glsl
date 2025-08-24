#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
void main() {
	FragColor = vec4(vertexColor.x, vertexColor.y, vertexColor.z, 1.0f);
}