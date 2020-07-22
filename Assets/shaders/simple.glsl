#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

vec4 lightPos = vec4(0.0, 0.0, -7.0, 1.0);

out vec2 TextureCoordinates;

out vec3 Normal;
out vec3 FragmentPosition;
out vec3 LightPosition;

void main() {
	TextureCoordinates = aTex;

	mat4 viewModelMatrix = u_view * u_model;
	vec4 localPos = viewModelMatrix * vec4(aPos, 1.0);

	Normal = normalize(mat3(transpose(inverse(viewModelMatrix))) * aNormal);
	FragmentPosition = vec3(localPos);
	LightPosition = vec3(u_view * lightPos);

	gl_Position = u_projection * localPos;
}



#type fragment

#version 330 core
out vec4 FragColor;

in vec2 TextureCoordinates;

in vec3 Normal;
in vec3 FragmentPosition;
in vec3 LightPosition;

uniform sampler2D textureSampler;

void main() {
	float ambient = 0.2;
	
	vec3 lightDir = normalize(LightPosition - FragmentPosition);
	float diffuse = max(dot(lightDir, Normal), 0.0);

	vec3 viewDir = normalize(-FragmentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specular = pow(max(dot(Normal, halfwayDir), 0.0), 32) * 0.5;

	FragColor = texture(textureSampler, TextureCoordinates) * (ambient + diffuse + specular);
	FragColor.xyz = pow(FragColor.xyz, vec3(1.0 / 2.2));
}
