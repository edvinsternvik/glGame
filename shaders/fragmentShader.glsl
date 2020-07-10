#version 330 core
out vec4 FragColor;

in vec2 TextureCoordinates;
in float lightDir;

uniform sampler2D textureSampler;

void main() {
	FragColor = texture(textureSampler, TextureCoordinates) * max((lightDir), 0.2);
}