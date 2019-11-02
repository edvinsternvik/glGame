#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
	vec3 pos = aPos * vec3(1, -1, 1);
	gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
}