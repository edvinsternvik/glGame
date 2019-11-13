#version 330 core
out vec4 FragColor;

in float lightDir;

void main() {
	FragColor = vec4(1.0, 0.0, 0.0, 1.0) * max((lightDir), 0.2);
}