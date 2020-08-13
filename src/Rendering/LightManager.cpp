#include "LightManager.h"
#include "Light.h"
#include "UniformBuffer.h"
#include "TextureArray.h"
#include "Shader.h"
#include "FrameBuffer.h"

#include <GL/glew.h>

#define UNIFORM_LIGHT_SIZE 112
#define MAX_LIGHTS 32
#define SHADOWMAP_MAX_SIZE 1024

namespace glGame {

    const unsigned int LightManager::MAX_SHADOWMAPS;

    LightManager::LightManager() {
        shadowmapShader = std::make_unique<Shader>(nullptr, 
			"#section vertex\n#version 330 core\nlayout (location = 0) in vec3 aPos;\nuniform mat4 u_lightSpaceMatrix;\nuniform mat4 u_model;\nvoid main() { gl_Position = u_lightSpaceMatrix * u_model * vec4(aPos, 1.0); }\n\n#section fragment\n#version 330 core\nvoid main() {}"
		);
		m_shadowmapTextureArray = std::make_shared<TextureArray>(MAX_SHADOWMAPS, SHADOWMAP_MAX_SIZE, SHADOWMAP_MAX_SIZE);

        unsigned int currentLightCount = m_lights.size();
		m_lightCountOffset = MAX_LIGHTS * UNIFORM_LIGHT_SIZE;
		m_lightsUniformBuffer = std::make_unique<UniformBuffer>(MAX_LIGHTS * UNIFORM_LIGHT_SIZE + 4);
		m_lightsUniformBuffer->addData(MAX_LIGHTS * UNIFORM_LIGHT_SIZE, NULL);
		m_lightsUniformBuffer->addData(4, &currentLightCount);
		m_lightsUniformBuffer->bindingPoint(1);

		for(int i = 0; i < MAX_SHADOWMAPS; ++i) {
			m_shadowmaps[i] = false;
		}
    }

    LightManager::~LightManager() {

    }

    void LightManager::updateLight(std::shared_ptr<Light> light) {
        if(m_lights.size() >= MAX_LIGHTS) return;

		if(light->shadowmapSize.x > SHADOWMAP_MAX_SIZE) light->shadowmapSize.x = SHADOWMAP_MAX_SIZE;
		if(light->shadowmapSize.y > SHADOWMAP_MAX_SIZE) light->shadowmapSize.y = SHADOWMAP_MAX_SIZE;

		// Create the shadowmap framebuffer
		if(light->castShadows && light->lightType == LightType::Directional) {
			if(!light->m_shadowmapFramebuffer.get() && m_shadowmapCount < MAX_SHADOWMAPS) {
				for(int i = 0; i < MAX_SHADOWMAPS; ++i) {
					if(m_shadowmaps[i] == false) {
						light->m_shadowmapId = i;
						break;
					}
				}
				m_shadowmaps[light->m_shadowmapId] = true;
				++m_shadowmapCount;
				light->m_shadowmapFramebuffer = std::make_unique<FrameBuffer>(m_shadowmapTextureArray, light->m_shadowmapId);
			}
		}
		else if(light->m_shadowmapFramebuffer.get()) {
			light->m_shadowmapFramebuffer = std::unique_ptr<FrameBuffer>();
			m_shadowmaps[light->m_shadowmapId] = false;
			light->m_shadowmapId = -1;
			++m_shadowmapCount;
		}

		// Define the data to send to the gpu
		struct UniformLightData {
			UniformLightData(Vector3& position, Vector3& direction, float& intensity, LightType& lightType, int& shadowmapId, glm::mat4& lightSpaceMatrix)
				: position(position), direction(direction), intensity(intensity), lightType(lightType), shadowmapId(shadowmapId), lightSpaceMatrix(lightSpaceMatrix) {}
			Vector3 position;
			float intensity;
			Vector3 direction;
			LightType lightType;
			int shadowmapId = -1;
			float pad1;
			float pad2;
			float pad3;
			glm::mat4 lightSpaceMatrix;
		};
		UniformLightData uniformLightData(light->position, light->direction, light->intensity, light->lightType, light->m_shadowmapId, light->m_lightSpaceMatrix);
		if(light->lightType == LightType::Directional) uniformLightData.position = light->direction;

		// Add Light if it hasn't allready be added, then increment the light counter
		m_lightsUniformBuffer->bindBuffer();
		if(light->m_lightId < 0) {
			light->m_lightId = m_lights.size();
			m_lights.push_back(light);

			unsigned int currentLightCount = m_lights.size();
			glBufferSubData(GL_UNIFORM_BUFFER, m_lightCountOffset, 4, &currentLightCount);
		}

		// Send the lights data to the gpu
		glBufferSubData(GL_UNIFORM_BUFFER, light->m_lightId * UNIFORM_LIGHT_SIZE, UNIFORM_LIGHT_SIZE, &uniformLightData);
		m_lightsUniformBuffer->unbindBuffer();
    }

    void LightManager::deleteLight(const unsigned int& lightid) {
        if(lightid > m_lights.size() - 1 || m_lights.size() < 1) {
			return;
		}

		auto lightToBeDeleted = m_lights[lightid];
		m_lightsUniformBuffer->bindBuffer();

		// Remove shadowmap
		if(lightToBeDeleted->castShadows && lightToBeDeleted->lightType == LightType::Directional) {
			m_shadowmaps[lightToBeDeleted->m_shadowmapId] = false;
			m_shadowmapCount--;
		}

		// When deleting a light, the last light in the array will be moved to take its place
		auto lastLight = m_lights.back();
		glCopyBufferSubData(GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER, lastLight->m_lightId * UNIFORM_LIGHT_SIZE, lightToBeDeleted->m_lightId * UNIFORM_LIGHT_SIZE, UNIFORM_LIGHT_SIZE);

		lastLight->m_lightId = lightid;
		lightToBeDeleted->m_lightId = -1;
		lightToBeDeleted->m_shadowmapId = -1;
		lightToBeDeleted->m_shadowmapFramebuffer = std::unique_ptr<FrameBuffer>();
		m_lights[lightid] = lastLight;
		m_lights.pop_back();

		unsigned int currentLightCount = m_lights.size();
		glBufferSubData(GL_UNIFORM_BUFFER, m_lightCountOffset, 4, &currentLightCount);
		m_lightsUniformBuffer->unbindBuffer();
    }

}