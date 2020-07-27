#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

layout (std140) uniform Camera {
	mat4 u_projection;
	mat4 u_view;
};

uniform mat4 u_model;

out vec2 TextureCoordinates;

out vec3 Normal;
out vec3 FragmentPosition;

void main() {
	TextureCoordinates = aTex;

	mat4 viewModelMatrix = u_view * u_model;
	vec4 localPos = viewModelMatrix * vec4(aPos, 1.0);

	Normal = normalize(mat3(transpose(inverse(viewModelMatrix))) * aNormal);
	FragmentPosition = vec3(localPos);

	gl_Position = u_projection * localPos;
}



#type fragment

#version 330 core
out vec4 FragColor;

layout (std140) uniform Camera {
	mat4 u_projection;
	mat4 u_view;
};

layout (std140) uniform Lights {
	vec3 u_lightPositions[32];
	uint u_lightCount;
};

uniform sampler2D textureSampler;

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

float calculatePointLight(uint lightId) {
	float ambient = 0.2;
	
	vec3 lightToFragmentVector = vec3(u_view * vec4(u_lightPositions[lightId], 1.0)) - FragmentPosition;
	vec3 lightDir = normalize(lightToFragmentVector);
	float diffuse = max(dot(lightDir, Normal), 0.0) * 4.0;

	vec3 viewDir = normalize(-FragmentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specular = pow(max(dot(Normal, halfwayDir), 0.0), 32) * 2.0;

	float lightToFragmentLength = length(lightToFragmentVector);
	float attenuation = 1.0 / (lightToFragmentLength * lightToFragmentLength);

	return (ambient + diffuse + specular) * attenuation;
}

void main() {
	float lighting = 0.0;
	for(uint i = uint(0); i < u_lightCount; ++i) {
		lighting += calculatePointLight(i);
	}

	FragColor = texture(textureSampler, TextureCoordinates) * (lighting);
	FragColor.xyz = pow(FragColor.xyz, vec3(1.0 / 2.2));
}
