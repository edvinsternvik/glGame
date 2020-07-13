#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 TextureCoordinates;

void main() {
	TextureCoordinates = aPos;

	mat4 view = mat4(mat3(u_view));
	vec4 pos = u_projection * view * vec4(aPos, 1.0);

	gl_Position = pos.xyww;
}


#type fragment

#version 330 core
out vec4 FragColor;

in vec3 TextureCoordinates;

uniform samplerCube cubemapSampler;

void main() {
	FragColor = texture(cubemapSampler, TextureCoordinates);
}