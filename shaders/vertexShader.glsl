#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform vec3 u_position;

void main() {
	vec3 pos = (aPos + u_position) * vec3(1, -1, 1);
	gl_Position = u_projection * u_view * vec4(pos, 1.0);
}