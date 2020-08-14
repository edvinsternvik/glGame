#include "Renderer.h"
#include "../Resources/Scene.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"
#include "Camera.h"
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

		bindDefaultRenderTarget();

		clearScreen();
	}

	void Renderer::render(Scene* scene, Camera* camera) {
		if(scene->activeCamera.expired()) return;

		const glm::mat4& projectionMatrix = camera->getProjectionMatrix();
		const glm::mat4& viewMatrix = camera->getViewMatrix();

		m_cameraUniformBuffer->setData(0, (void*)&(projectionMatrix[0][0]));
		m_cameraUniformBuffer->setData(1, (void*)&(viewMatrix[0][0]));
		
		// Render scene
		frameRenderData.clear();
		for(int i = 0; i < scene->getGameObjectCount(); ++i) {
			GameObject* gameObject = scene->getGameObject(i).lock().get();

			m_objectRenderData.vao = nullptr;
			m_objectRenderData.material = nullptr;
			for(int j = 0; j < gameObject->getComponentSize(); ++j) {
				gameObject->getComponent(j)->onRender();
			}
			m_objectRenderData.gameObjectId = i;

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
		glActiveTexture(GL_TEXTURE2);
		m_lightManager->m_shadowmapTextureArray->bind();

		glViewport(0, 0, viewportSize.x, viewportSize.y);
		bindDefaultRenderTarget();

		renderObjects(frameRenderData);

		if(m_skyboxRenderData.cubemap && m_skyboxRenderData.shader) {
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);
			
			m_skyboxRenderData.shader->useShader();
			glActiveTexture(GL_TEXTURE0);
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
		unbindDefaultRenderTarget();
	}

	void Renderer::setDefaultRenderTarget(std::shared_ptr<FrameBuffer> renderTarget) {
		m_defaultRenderTarget = renderTarget;
	}

	void Renderer::renderObjects(std::vector<ObjectRenderData>& renderData) {
		for(ObjectRenderData& objRenderData : renderData) {
			if(!objRenderData.material->shader.expired()) {
				objRenderData.material->shader->useShader();

				int hasTexture = bindTexture(objRenderData.material->texture.get(), objRenderData.material->shader.get(), "textureSampler", 0);
				int hasSpecularMap = bindTexture(objRenderData.material->specularMap.get(), objRenderData.material->shader.get(), "specularSampler", 1);
				objRenderData.material->shader->setUniform1i("u_hasTexture", hasTexture);
				objRenderData.material->shader->setUniform1i("u_hasSpecularMap", hasSpecularMap);
				objRenderData.material->shader->setUniform1i("shadowMap", 2);

				prepareRenderingConfiguration(objRenderData.material->shader.get());
				
				objRenderData.vao->bind();
				objRenderData.material->shader->setUniformMat4("u_model", &(objRenderData.modelMatrix[0][0]));
				glDrawArrays(GL_TRIANGLES, 0, objRenderData.verticies);

				revertRenderingConfiguration(objRenderData.material->shader.get());
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	bool Renderer::bindTexture(Texture* texture, Shader* shader, const char* samplerName, int textureUnit) {
		if(shader) shader->setUniform1i(samplerName, textureUnit);
		if(texture) {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			texture->bind();
			return true;
		}
		return false;
	}

	void Renderer::prepareRenderingConfiguration(Shader* shader) {
		if(!shader->m_depthTestingEnabled) glDepthFunc(GL_ALWAYS);
		if(shader->m_stencilEnabled) {
			glEnable(GL_STENCIL_TEST);
			if(shader->m_stencilWriteEnabled) glStencilMask(0xFF);
			else glStencilMask(0x00);
			glStencilFunc(Shader::GetOpenGLStencilFunc(shader->m_stencilFunc), shader->m_stencilFuncRef, 0xFF);
			glStencilOp(Shader::GetOpenGLStencilOp(shader->m_stencilOpSFail), Shader::GetOpenGLStencilOp(shader->m_stencilOpDPFail), Shader::GetOpenGLStencilOp(shader->m_stencilOpDPPass));
		}
	}

	void Renderer::revertRenderingConfiguration(Shader* shader) {
		if(!shader->m_depthTestingEnabled) glDepthFunc(GL_LESS);
		if(shader->m_stencilEnabled) glDisable(GL_STENCIL_TEST);
	}

	
	void Renderer::bindDefaultRenderTarget() {
		if(m_defaultRenderTarget.get()) {
			m_defaultRenderTarget->bind();
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void Renderer::unbindDefaultRenderTarget() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
