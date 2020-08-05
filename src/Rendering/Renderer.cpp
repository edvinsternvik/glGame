#include "Renderer.h"
#include "../Resources/Scene.h"
#include "../Components/Camera.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"
#include "Cubemap.h"
#include "Material.h"
#include "Light.h"
#include "UniformBuffer.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define UNIFORM_LIGHT_SIZE 32

namespace glGame {

	void Renderer::init(Vector2i viewportSize) {
		initGLEW();

		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer = std::make_unique<FrameBuffer>(1280, 720);
		#endif
		std::shared_ptr<Texture> framebufferTexture = std::make_shared<Texture>(1024, 1024, TextureType::DEPTH);
		m_shadowFramebuffer = std::make_unique<FrameBuffer>(framebufferTexture);
		m_shadowShader = std::make_unique<Shader>(nullptr, 
			"#type vertex\n#version 330 core\nlayout (location = 0) in vec3 aPos;\nuniform mat4 u_lightSpaceMatrix;\nuniform mat4 u_model;\nvoid main() { gl_Position = u_lightSpaceMatrix * u_model * vec4(aPos, 1.0); }\n\n#type fragment\n#version 330 core\nvoid main() {}"
		);

		m_cameraUniformBuffer = std::make_unique<UniformBuffer>(128);
		m_cameraUniformBuffer->addData(64, NULL);
		m_cameraUniformBuffer->addData(64, NULL);
		m_cameraUniformBuffer->bindingPoint(0);

		unsigned int lightCount = 32;
		m_lightCountOffset = lightCount * UNIFORM_LIGHT_SIZE;
		m_lightsUniformBuffer = std::make_unique<UniformBuffer>(lightCount * UNIFORM_LIGHT_SIZE + 4);
		m_lightsUniformBuffer->addData(lightCount * UNIFORM_LIGHT_SIZE, NULL);
		m_lightsUniformBuffer->addData(4, &m_lightCount);
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

	unsigned int Renderer::addLight(const Light& light) {
		m_lightOffsets[m_lightIdCount] = m_lightCount * UNIFORM_LIGHT_SIZE;

		m_lightsUniformBuffer->bindBuffer();
		glBufferSubData(GL_UNIFORM_BUFFER, m_lightCount * UNIFORM_LIGHT_SIZE, sizeof(Light), &light);
		++m_lightCount;
		glBufferSubData(GL_UNIFORM_BUFFER, m_lightCountOffset, 4, &m_lightCount);
		m_lightsUniformBuffer->unbindBuffer();
		return m_lightIdCount++;
	}

	void Renderer::updateLight(const unsigned int& lightId, const Light& light) {
		auto lightOffset = m_lightOffsets.find(lightId);
		if(lightOffset == m_lightOffsets.end()) {
			return;
		}

		m_lightsUniformBuffer->bindBuffer();
		glBufferSubData(GL_UNIFORM_BUFFER, lightOffset->second, sizeof(Light), &light);
		m_lightsUniformBuffer->unbindBuffer();
	}

	void Renderer::deleteLight(const unsigned int& lightid) {
		auto lightOffset = m_lightOffsets.find(lightid);
		if(lightOffset == m_lightOffsets.end()) {
			return;
		}

		unsigned int lastLightId = -1;
		for(auto lightOffsetIterator : m_lightOffsets) {
			if(lightOffsetIterator.second == (m_lightCount - 1) * UNIFORM_LIGHT_SIZE) {
				lastLightId = lightOffsetIterator.first;
				break;
			}
		}
		if(lastLightId == -1) return;

		m_lightsUniformBuffer->bindBuffer();
		glCopyBufferSubData(GL_UNIFORM_BUFFER, GL_UNIFORM_BUFFER, m_lightOffsets[lastLightId], lightOffset->second, UNIFORM_LIGHT_SIZE);

		m_lightOffsets[lastLightId] = lightOffset->second;

		--m_lightCount;
		glBufferSubData(GL_UNIFORM_BUFFER, m_lightCountOffset, 4, &m_lightCount);
		m_lightsUniformBuffer->unbindBuffer();
		m_lightOffsets.erase(lightid);
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
		
		//Render scene
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

		glViewport(0, 0, 1024, 1024);
		m_shadowFramebuffer->bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 24.0f);
		glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 2.0f, -1.0f), glm::vec3( 0.0f, -4.0f,  -10.0f), glm::vec3( 0.0f, 1.0f,  0.0f)); 
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		m_shadowShader->useShader();
		m_shadowShader->setUniformMat4("u_lightSpaceMatrix", &(lightSpaceMatrix[0][0]));
		
		glCullFace(GL_FRONT);
		renderObjectsShadow(frameRenderData);
		glCullFace(GL_BACK);

		m_shadowFramebuffer->unbind();
		glViewport(0, 0, viewportSize.x, viewportSize.y);
		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		for(ObjectRenderData& objRenderData : frameRenderData) {
			if(objRenderData.vao) {
				if(objRenderData.material && !objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
					objRenderData.material->shader->useShader();
					objRenderData.material->shader->setUniformMat4("u_lightSpaceMatrix", &(lightSpaceMatrix[0][0]));
				}
			}
		}
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

					glActiveTexture(GL_TEXTURE2);
					objRenderData.material->shader->setUniform1i("shadowMap", 2);
					m_shadowFramebuffer->getTexture()->bind();
					
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
				m_shadowShader->useShader();
				objRenderData.vao->bind();
				m_shadowShader->setUniformMat4("u_model", &(objRenderData.modelMatrix[0][0]));
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
