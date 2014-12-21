#version 330 core

layout(location = 0) in vec3 vertex;
out vec3 texCoord;
uniform mat4 PVM;

void main() {
	gl_Position = PVM * vec4(vertex, 1.0);
	texCoord = vertex;
}