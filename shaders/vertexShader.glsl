#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

vec3 lightPos = vec3(4.0, 4.0, 4.0);

out float lightDir;

void main() {
//	lightDir = 1.0;
	vec4 pos = u_model * vec4(aPos, 1.0);
	vec3 hello = normalize(lightPos - vec3(pos));
	lightDir = dot(aNormal, hello);

	gl_Position = u_projection * u_view * pos;
}