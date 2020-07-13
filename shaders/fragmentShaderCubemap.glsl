#version 330 core
out vec4 FragColor;

in vec3 TextureCoordinates;

uniform samplerCube cubemapSampler;

void main() {
	FragColor = texture(cubemapSampler, TextureCoordinates);
}