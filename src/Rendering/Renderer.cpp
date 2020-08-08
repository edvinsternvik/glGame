#include "Renderer.h"
#include "../Resources/Scene.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"
#include "Cubemap.h"
#include "Material.h"
#include "Light.h"
#include "UniformBuffer.h"
#include "TextureArray.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define UNIFORM_LIGHT_SIZE 112
#define MAX_LIGHTS 32

namespace glGame {

	void Renderer::init(Vector2i viewportSize) {
		initGLEW();

		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer = std::make_unique<FrameBuffer>(1280, 720);
		#endif
		// std::shared_ptr<Texture> framebufferTexture = std::make_shared<Texture>(1024, 1024, TextureType::DEPTH);
		// m_shadowFramebuffer = std::make_unique<FrameBuffer>(framebufferTexture);
		m_shadowmapShader = std::make_unique<Shader>(nullptr, 
			"#type vertex\n#version 330 core\nlayout (location = 0) in vec3 aPos;\nuniform mat4 u_lightSpaceMatrix;\nuniform mat4 u_model;\nvoid main() { gl_Position = u_lightSpaceMatrix * u_model * vec4(aPos, 1.0); }\n\n#type fragment\n#version 330 core\nvoid main() {}"
		);

		m_cameraUniformBuffer = std::make_unique<UniformBuffer>(128);
		m_cameraUniformBuffer->addData(64, NULL);
		m_cameraUniformBuffer->addData(64, NULL);
		m_cameraUniformBuffer->bindingPoint(0);

		unsigned int currentLightCount = m_lights.size();
		m_lightCountOffset = MAX_LIGHTS * UNIFORM_LIGHT_SIZE;
		m_lightsUniformBuffer = std::make_unique<UniformBuffer>(MAX_LIGHTS * UNIFORM_LIGHT_SIZE + 4);
		m_lightsUniformBuffer->addData(MAX_LIGHTS * UNIFORM_LIGHT_SIZE, NULL);
		m_lightsUniformBuffer->addData(4, &currentLightCount);
		m_lightsUniformBuffer->bindingPoint(1);

		this->viewportSize = viewportSize;
	}

	void Renderer::submit(Model* model, const glm::mat4& modelMatrix) {
		m_objectRenderData = ObjectRenderData(model->getVertexArray(), model->getVerticiesCount(), m_objectRenderData.material, modelMatrix);
	}

	void Renderer::submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix) {
		m_objectRenderData = ObjectRenderData(vertexArray, verticies, m_objectRenderData.material, modelMatrix);
	}

	void Renderer::submit(Cubemap* cubemap, Shader* shader) {
		m_skyboxRenderData = SkyboxRenderData(cubemap, shader);
	}

	void Renderer::updateLight(std::shared_ptr<Light> light) {
		if(m_lights.size() >= MAX_LIGHTS) return;

		struct UniformLightData {
			UniformLightData(Vector3& position, Vector3& direction, float& intensity, LightType& lightType, glm::mat4& lightSpaceMatrix)
				: position(position), direction(direction), intensity(intensity), lightType(lightType), lightSpaceMatrix(lightSpaceMatrix) {}
			Vector3 position;
			float padding1;
			Vector3 direction;
			float intensity;
			LightType lightType;
			glm::mat4 lightSpaceMatrix;
		};
		UniformLightData uniformLightData(light->position, light->direction, light->intensity, light->lightType, light->m_lightSpaceMatrix);
		if(light->lightType == LightType::Directional) uniformLightData.position = light->direction;

		m_lightsUniformBuffer->bindBuffer();
		if(light->m_lightId < 0) {
			light->m_lightId = m_lights.size();
			m_lights.push_back(light);

			unsigned int currentLightCount = m_lights.size();
			glBufferSubData(GL_UNIFORM_BUFFER, m_lightCountOffset, 4, &currentLightCount);
		}

		glBufferSubData(GL_UNIFORM_BUFFER, light->m_lightId * UNIFORM_LIGHT_SIZE, UNIFORM_LIGHT_SIZE, &uniformLightData);
		m_lightsUniformBuffer->unbindBuffer();
	}

	void Renderer::deleteLight(const unsigned int& lightid) {
		if(lightid > m_lights.size() - 1 || m_lights.size() < 1) {
			return;
		}

		auto lightToBeDeleted = m_lights[lightid], lastLight = m_lights.back();

		m_lightsUniformBuffer->bindBuffer();
		glCopyBufferSubData(GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER, lastLight->m_lightId * UNIFORM_LIGHT_SIZE, lightToBeDeleted->m_lightId * UNIFORM_LIGHT_SIZE, UNIFORM_LIGHT_SIZE);

		lastLight->m_lightId = lightid;
		lightToBeDeleted->m_lightId = -1;
		m_lights[lightid] = lastLight;
		m_lights.pop_back();

		unsigned int currentLightCount = m_lights.size();
		glBufferSubData(GL_UNIFORM_BUFFER, m_lightCountOffset, 4, &currentLightCount);
		m_lightsUniformBuffer->unbindBuffer();
	}

	void Renderer::setMaterial(Material* material) {
		m_objectRenderData.material = material;
	}

	void Renderer::beginRender() {
		clearScreen();

		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		clearScreen();
	}

	void Renderer::render(Scene* scene) {
		if(scene->activeCamera.expired()) return;

		const glm::mat4& projectionMatrix = scene->activeCamera.lock()->getProjectionMatrix();
		const glm::mat4& viewMatrix = scene->activeCamera.lock()->getViewMatrix();

		m_cameraUniformBuffer->setData(0, (void*)&(projectionMatrix[0][0]));
		m_cameraUniformBuffer->setData(1, (void*)&(viewMatrix[0][0]));
		
		// Render scene
		std::vector<ObjectRenderData> frameRenderData;
		for(int i = 0; i < scene->getGameObjectCount(); ++i) {
			GameObject* gameObject = scene->getGameObject(i);

			m_objectRenderData.vao = nullptr;
			m_objectRenderData.material = nullptr;
			for(int j = 0; j < gameObject->getComponentSize(); ++j) {
				gameObject->getComponent(j)->onRender();
			}

			processRenderData(frameRenderData);
		}

		// Render shadowmap
		for(auto& light : m_lights) {
			if(light->lightType == LightType::Directional && light->castShadows) {
				glViewport(0, 0, light->shadowmapSize.x, light->shadowmapSize.y);
				light->m_shadowmapFramebuffer->bind();
				glClear(GL_DEPTH_BUFFER_BIT);
				m_shadowmapShader->useShader();
				m_shadowmapShader->setUniformMat4("u_lightSpaceMatrix", &(light->m_lightSpaceMatrix[0][0]));
				glActiveTexture(GL_TEXTURE2);
				light->m_shadowmapFramebuffer->getTextureArray()->bind();

				glCullFace(GL_FRONT);
				renderObjectsShadow(frameRenderData);
				glCullFace(GL_BACK);

				light->m_shadowmapFramebuffer->unbind();
			}
		}
		glViewport(0, 0, viewportSize.x, viewportSize.y);
		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		// TEMPORARY
		for(ObjectRenderData& objRenderData : frameRenderData) {
			if(objRenderData.vao) {
				if(objRenderData.material && !objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
					objRenderData.material->shader->useShader();
					objRenderData.material->shader->setUniformMat4("u_lightSpaceMatrix", &(m_lights[2]->m_lightSpaceMatrix[0][0]));
				}
			}
		}
		// ----
		renderObjects(frameRenderData);

		if(m_skyboxRenderData.cubemap && m_skyboxRenderData.shader) {
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			
			m_skyboxRenderData.shader->useShader();
			m_skyboxRenderData.shader->setUniformMat4("u_projection", &(projectionMatrix[0][0]));
			m_skyboxRenderData.shader->setUniformMat4("u_view", &(viewMatrix[0][0]));
			m_skyboxRenderData.cubemap->bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);

			m_skyboxRenderData = SkyboxRenderData();
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
		}
	}

	void Renderer::renderGizmos(const std::vector<GameObject*>& gizmoObjects) {
		glClear(GL_DEPTH_BUFFER_BIT);
		for(GameObject* go : gizmoObjects) {
			go->onRender();
		}
	}

	void Renderer::endRender() {
		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer->unbind();
		#endif
	}

	unsigned int Renderer::getEditorFrameTexture() {
		return m_editorFramebuffer->getTexture()->getTextureId();
	}

	void Renderer::renderObjects(std::vector<ObjectRenderData>& renderData) {
		for(ObjectRenderData& objRenderData : renderData) {
			if(objRenderData.vao) {
				if(objRenderData.material && !objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
					objRenderData.material->shader->useShader();

					glActiveTexture(GL_TEXTURE0);
					objRenderData.material->texture->bind();
					if(objRenderData.material->specularMap.expired()) {
						objRenderData.material->shader->setUniform1i("specularSampler", -1);
					}
					else {
						objRenderData.material->shader->setUniform1i("specularSampler", 1);
						glActiveTexture(GL_TEXTURE1);
						objRenderData.material->specularMap->bind();
					}

					objRenderData.material->shader->setUniform1i("shadowMap", 2);
					
					objRenderData.vao->bind();
					objRenderData.material->shader->setUniformMat4("u_model", &(objRenderData.modelMatrix[0][0]));
					glDrawArrays(GL_TRIANGLES, 0, objRenderData.verticies);
				}

			}
		}
	}

	void Renderer::renderObjectsShadow(std::vector<ObjectRenderData>& renderData) {
		for(ObjectRenderData& objRenderData : renderData) {
			if(objRenderData.vao && objRenderData.material && !objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
				m_shadowmapShader->useShader();
				objRenderData.vao->bind();
				m_shadowmapShader->setUniformMat4("u_model", &(objRenderData.modelMatrix[0][0]));
				glDrawArrays(GL_TRIANGLES, 0, objRenderData.verticies);
			}
		}
	}

	void Renderer::initGLEW() {
		if (glewInit() != GLEW_OK) {
			std::cout << "Could not initialize GLEW" << std::endl;
			return;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::processRenderData(std::vector<ObjectRenderData>& frameRenderData) {
		frameRenderData.push_back(m_objectRenderData);
	}

	void Renderer::clearScreen() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}
