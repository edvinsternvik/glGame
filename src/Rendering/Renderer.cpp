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
#include "LightManager.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

	Renderer::Renderer() {

	}

	Renderer::~Renderer() {

	}

	void Renderer::init(Vector2i viewportSize) {
		initGLEW();

		#ifdef GL_GAME_EDITOR
		m_editorFramebuffer = std::make_unique<FrameBuffer>(1280, 720);
		#endif

		m_lightManager = std::make_unique<LightManager>();

		m_cameraUniformBuffer = std::make_unique<UniformBuffer>(128);
		m_cameraUniformBuffer->addData(64, NULL);
		m_cameraUniformBuffer->addData(64, NULL);
		m_cameraUniformBuffer->bindingPoint(0);

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
		m_lightManager->updateLight(light);
	}

	void Renderer::deleteLight(const unsigned int& lightid) {
		m_lightManager->deleteLight(lightid);
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
		frameRenderData.clear();
		for(int i = 0; i < scene->getGameObjectCount(); ++i) {
			GameObject* gameObject = scene->getGameObject(i);

			m_objectRenderData.vao = nullptr;
			m_objectRenderData.material = nullptr;
			for(int j = 0; j < gameObject->getComponentSize(); ++j) {
				gameObject->getComponent(j)->onRender();
			}

			processRenderData(frameRenderData);
		}

		// Render shadowmaps
		for(auto& light : m_lightManager->m_lights) {
			if(light->m_shadowmapFramebuffer.get()) {
				light->m_shadowmapFramebuffer->bind();
				m_lightManager->shadowmapShader->useShader();
				m_lightManager->shadowmapShader->setUniformMat4("u_lightSpaceMatrix", &(light->m_lightSpaceMatrix[0][0]));

				glViewport(0, 0, light->shadowmapSize.x, light->shadowmapSize.y);
				glClear(GL_DEPTH_BUFFER_BIT);
				glCullFace(GL_FRONT);
				renderObjectsShadow(frameRenderData);
				glCullFace(GL_BACK);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE2);
		m_lightManager->m_shadowmapTextureArray->bind();

		glViewport(0, 0, viewportSize.x, viewportSize.y);
		#ifdef GL_GAME_EDITOR
		//Render to frame texture
		m_editorFramebuffer->bind();
		#endif

		renderObjects(frameRenderData);

		if(m_skyboxRenderData.cubemap && m_skyboxRenderData.shader) {
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			
			m_skyboxRenderData.shader->useShader();
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
			if(!objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
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

	void Renderer::renderObjectsShadow(std::vector<ObjectRenderData>& renderData) {
		for(ObjectRenderData& objRenderData : renderData) {
			if(!objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
				m_lightManager->shadowmapShader->useShader();
				objRenderData.vao->bind();
				m_lightManager->shadowmapShader->setUniformMat4("u_model", &(objRenderData.modelMatrix[0][0]));
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
		if(m_objectRenderData.vao && m_objectRenderData.material) frameRenderData.push_back(m_objectRenderData);
	}

	void Renderer::clearScreen() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
}
