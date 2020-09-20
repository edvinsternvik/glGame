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

	void Renderer::submit(Model* model, const glm::mat4& modelMatrix, Material* material, const int& layer) {
		m_renderDataList.insert(layer,
			ObjectRenderData(model->getVertexArray(), model->getVerticiesCount(), material, modelMatrix, {})
		);
	}

	void Renderer::submit(Model* model, const glm::mat4& modelMatrix, Material* material, const UniformArray& uniforms, const int& layer) {
		m_renderDataList.insert(layer,
			ObjectRenderData(model->getVertexArray(), model->getVerticiesCount(), material, modelMatrix, uniforms)
		);
	}

	void Renderer::submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix, Material* material, const int& layer) {
		m_renderDataList.insert(layer, 
			ObjectRenderData(vertexArray, verticies, material, modelMatrix, {})
		);
	}

	void Renderer::submit(Model* model, const glm::mat4& modelMatrix, Shader* shader, const int& layer) {
		m_renderDataList.insert(layer,
			ObjectRenderData(model->getVertexArray(), model->getVerticiesCount(), shader, modelMatrix, {})
		);
	}

	void Renderer::submit(Model* model, const glm::mat4& modelMatrix, Shader* shader, const UniformArray& uniforms, const int& layer) {
		m_renderDataList.insert(layer,
			ObjectRenderData(model->getVertexArray(), model->getVerticiesCount(), shader, modelMatrix, uniforms)
		);
	}

	void Renderer::submit(const ObjectRenderData& objRenderData, const int& layer) {
		m_renderDataList.insert(layer, objRenderData);
	}

	void Renderer::submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix, Shader* shader, const int& layer) {
		m_renderDataList.insert(layer, ObjectRenderData(vertexArray, verticies, shader, modelMatrix, {}));
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
		for(int i = 0; i < scene->getGameObjectCount(); ++i) {
			GameObject* gameObject = scene->getGameObject(i).lock().get();

			int frameRenderDataSize = m_renderDataList.size();
			for(int j = 0; j < gameObject->getComponentSize(); ++j) {
				gameObject->getComponent(j)->onRender(); // Rendering components(MeshRenderer etc) submit renderingData through this method
				if(frameRenderDataSize < m_renderDataList.size()) { // If new renderingData has been submitted
					m_renderDataList.last()->gameObjectId = i;
				}
			}
		}

		// Render shadowmaps
		for(auto& light : m_lightManager->m_lights) {
			if(light->m_shadowmapFramebuffer.get()) {
				light->m_shadowmapFramebuffer->bind();
				m_lightManager->shadowmapShader->useShader();
				m_lightManager->shadowmapShader->setUniformMat4("u_lightSpaceMatrix", light->m_lightSpaceMatrix);

				glViewport(0, 0, light->shadowmapSize.x, light->shadowmapSize.y);
				glClear(GL_DEPTH_BUFFER_BIT);
				glCullFace(GL_FRONT);
				renderObjectsShadow(m_renderDataList);
				glCullFace(GL_BACK);
			}
		}
		glActiveTexture(GL_TEXTURE2);
		m_lightManager->m_shadowmapTextureArray->bind();

		glViewport(0, 0, viewportSize.x, viewportSize.y);
		bindDefaultRenderTarget();

		bool firstLayer = true;
		for(ObjectRenderDataLayer& renderDataLayer : m_renderDataList) {
			clearDepthAndStencil();
			renderObjects(renderDataLayer);

			if(firstLayer) renderSkybox();
			firstLayer = false;
		}

		previousFrameRenderData = m_renderDataList;
		m_renderDataList.clear();
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

	void Renderer::renderObjects(ObjectRenderDataLayer& renderDataLayer) {
		for(ObjectRenderData& objRenderData : renderDataLayer) {
			Shader* shader;
			int hasTexture = 0, hasSpecularMap = 0;
			if(objRenderData.material && !objRenderData.material->shader.expired()) {
				shader = objRenderData.material->shader.get();

				hasTexture = bindTexture(objRenderData.material->texture.get(), shader, "textureSampler", 0);
				hasSpecularMap = bindTexture(objRenderData.material->specularMap.get(), shader, "specularSampler", 1);
			}
			else if(objRenderData.shader) {
				shader = objRenderData.shader;
			}
			else {
				return;
			}
			shader->useShader();

			shader->setUniform1i("u_hasTexture", hasTexture);
			shader->setUniform1i("u_hasSpecularMap", hasSpecularMap);
			shader->setUniform1i("shadowMap", 2);

			prepareRenderingConfiguration(shader);
			for(auto& uniform : objRenderData.uniformData) uniform.setUniform(shader);
			
			objRenderData.vao->bind();
			shader->setUniformMat4("u_model", objRenderData.modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, objRenderData.verticies);

			revertRenderingConfiguration(shader);
		}
	}

	void Renderer::renderObjectsShadow(RenderDataList& renderData) {
		for(ObjectRenderDataLayer& renderDataLayer : m_renderDataList) {
			for(ObjectRenderData& objRenderData : renderDataLayer) {
				if(objRenderData.material && !objRenderData.material->texture.expired() && !objRenderData.material->shader.expired()) {
					m_lightManager->shadowmapShader->useShader();
					objRenderData.vao->bind();
					m_lightManager->shadowmapShader->setUniformMat4("u_model", objRenderData.modelMatrix);
					glDrawArrays(GL_TRIANGLES, 0, objRenderData.verticies);
				}
			}
		}
	}

	void Renderer::renderSkybox() {
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

	void Renderer::initGLEW() {
		if (glewInit() != GLEW_OK) {
			std::cout << "Could not initialize GLEW" << std::endl;
			return;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::clearScreen() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::clearDepthAndStencil() {
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
		if(!shader->m_colorEnabled) glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		if(!shader->m_depthTestingEnabled) glDisable(GL_DEPTH_TEST);
		if(!shader->m_depthTestWriteEnabled) glDepthMask(GL_FALSE);
		if(shader->m_depthTestFunc != CompareFunc::Less) glDepthFunc(Shader::GetOpenGLCompareFunc(shader->m_depthTestFunc));
		if(shader->m_stencilEnabled) {
			glEnable(GL_STENCIL_TEST);
			if(shader->m_stencilWriteEnabled) glStencilMask(0xFF);
			else glStencilMask(0x00);
			glStencilFunc(Shader::GetOpenGLCompareFunc(shader->m_stencilFunc), shader->m_stencilFuncRef, 0xFF);
			glStencilOp(Shader::GetOpenGLStencilOp(shader->m_stencilOpSFail), Shader::GetOpenGLStencilOp(shader->m_stencilOpDPFail), Shader::GetOpenGLStencilOp(shader->m_stencilOpDPPass));
		}
	}

	void Renderer::revertRenderingConfiguration(Shader* shader) {
		if(!shader->m_colorEnabled) glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		if(!shader->m_depthTestingEnabled) glEnable(GL_DEPTH_TEST);
		if(!shader->m_depthTestWriteEnabled) glDepthMask(GL_TRUE);
		if(shader->m_depthTestFunc != CompareFunc::Less) glDepthFunc(GL_LESS);
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
