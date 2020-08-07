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
uniform mat4 u_lightSpaceMatrix;

out vec2 TextureCoordinates;

out vec3 Normal;
out vec3 FragmentPosition;
out vec4 FragPosLightSpace;

void main() {
	TextureCoordinates = aTex;

	mat4 viewModelMatrix = u_view * u_model;
	vec4 localPos = viewModelMatrix * vec4(aPos, 1.0);

	Normal = normalize(mat3(transpose(inverse(viewModelMatrix))) * aNormal);
	FragmentPosition = vec3(localPos);
	FragPosLightSpace = u_lightSpaceMatrix * u_model * vec4(aPos, 1.0);

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
	vec3 direction;
	float intensity;
	uint lightType;
	float padding1;
	float padding2;
	mat4 lightSpaceMatrix;
};

layout (std140) uniform Lights {
	Light u_lights[32];
	uint u_lightCount;
};

uniform sampler2D textureSampler;
uniform sampler2D specularSampler;
uniform sampler2D shadowMap;

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;
in vec4 FragPosLightSpace;

float calculatePointLight(uint lightId);
float calculateDirectionalLight(uint lightId);
float calculateShadow(vec4 lightSpacePos, vec3 lightDir);

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
	specular *= texture(specularSampler, TextureCoordinates).r;

	float lightToFragmentLength = length(lightToFragmentVector);
	float attenuation = 1.0 / (lightToFragmentLength * lightToFragmentLength);

	return (ambient + diffuse + specular) * attenuation * u_lights[lightId].intensity;
}

float calculateDirectionalLight(uint lightId) {
	float ambient = 0.1;
	
	vec3 lightDir = normalize(vec3(u_view * -vec4(u_lights[lightId].direction, 0.0)));
	float diffuse = max(dot(lightDir, Normal), 0.0);

	vec3 viewDir = normalize(-FragmentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float specular = pow(max(dot(Normal, halfwayDir), 0.0), 32) * 0.5;
	specular *= texture(specularSampler, TextureCoordinates).r;

	float shadow = calculateShadow(FragPosLightSpace, lightDir);

	return (ambient + (1.0 - shadow) * (diffuse + specular)) * u_lights[lightId].intensity;
}

float calculateShadow(vec4 lightSpacePos, vec3 lightDir) {
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	if(currentDepth > 1.0) return 0.0;

	float bias = max(0.005 * (1.0 - dot(lightDir, -Normal)), 0.0001);
	float shadow = 0.0;

	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
		}	
	}
	shadow /= 9.0;

	return shadow;
}
