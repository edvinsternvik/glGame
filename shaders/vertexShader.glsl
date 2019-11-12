#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
	vec3 pos = aPos;
	gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
}