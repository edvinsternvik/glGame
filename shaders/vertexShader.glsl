#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 u_position;

void main() {
	gl_Position = vec4((aPos + u_position) * vec3(1, -1, 1), 1.0);
}