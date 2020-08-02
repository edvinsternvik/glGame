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

struct Light {
	vec3 position;
	float intensity;
	uint lightType;
	float padding1;
	float padding2;
};

layout (std140) uniform Lights {
	Light u_lights[32];
	uint u_lightCount;
};

uniform sampler2D textureSampler;
uniform sampler2D specularSampler;

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

float calculatePointLight(uint lightId);
float calculateDirectionalLight(uint lightId);

void main() {
	float lighting = 0.0;
	for(uint i = uint(0); i < u_lightCount; ++i) {
		if(u_lights[i].lightType == uint(0)) lighting += calculatePointLight(i);
		else if(u_lights[i].lightType == uint(1)) lighting += calculateDirectionalLight(i);
	}

	FragColor = texture(textureSampler, TextureCoordinates) * lighting;
	FragColor.xyz = pow(FragColor.xyz, vec3(1.0 / 2.2));
}

float calculatePointLight(uint lightId) {
	float ambient = 0.2;
	
	vec3 lightToFragmentVector = vec3(u_view * vec4(u_lights[lightId].position, 1.0)) - FragmentPosition;
	vec3 lightDir = normalize(lightToFragmentVector);
	float diffuse = max(dot(lightDir, Normal), 0.0) * 4.0;

	vec3 viewDir = normalize(-FragmentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specular = pow(max(dot(Normal, halfwayDir), 0.0), 32) * 2.0;
	vec3 specMap = texture(specularSampler, TextureCoordinates).rgb;
	specular *= specMap.r + specMap.g + specMap.b;

	float lightToFragmentLength = length(lightToFragmentVector);
	float attenuation = 1.0 / (lightToFragmentLength * lightToFragmentLength);

	return (ambient + diffuse + specular) * attenuation * u_lights[lightId].intensity;
}

float calculateDirectionalLight(uint lightId) {
	float ambient = 0.2;
	
	vec3 lightDir = normalize(vec3(u_view * vec4(u_lights[lightId].position, 0.0)));
	float diffuse = max(dot(lightDir, Normal), 0.0);

	vec3 viewDir = normalize(-FragmentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specular = pow(max(dot(Normal, halfwayDir), 0.0), 32) * 0.5;
	vec3 specMap = texture(specularSampler, TextureCoordinates).rgb;
	specular *= specMap.r + specMap.g + specMap.b;

	return (ambient + diffuse + specular) * u_lights[lightId].intensity;
}
