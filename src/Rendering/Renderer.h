#pragma once
#include "FrameBuffer.h"
#include "Shader.h"
#include "Model.h"
#include "UniformBuffer.h"
#include "../Math/Vector.h"
#include "RenderDataList.h"
#include <GL/glew.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace glGame {

	class Scene;
	class GameObject;
	class Camera;
	class Cubemap;
	class Light;
	class LightManager;

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void init(Vector2i viewportSize);

		void submit(Model* model, const glm::mat4& modelMatrix, Material* material, const int& layer = 0);
		void submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix, Material* material, const int& layer = 0);
		void submit(Model* model, const glm::mat4& modelMatrix, Shader* shader, const int& layer = 0);
		void submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix, Shader* shader, const int& layer = 0);
		void submit(Cubemap* cubemap, Shader* shader);
		void updateLight(std::shared_ptr<Light> light);
		void deleteLight(const unsigned int& lightid);

		void beginRender();
		void render(Scene* scene, Camera* camera);
		void renderGizmos(const std::vector<GameObject*>& gizmoObjects);
		void endRender();

		void setDefaultRenderTarget(std::shared_ptr<FrameBuffer> renderTarget);

	private:

		struct SkyboxRenderData {
			SkyboxRenderData() : cubemap(nullptr), shader(nullptr) {}
			SkyboxRenderData(Cubemap* cubemap, Shader* shader) : cubemap(cubemap), shader(shader) {}
			Cubemap* cubemap;
			Shader* shader;
		};

		void renderObjects(RenderDataList& renderData);
		void renderObjectsShadow(RenderDataList& renderData);
		void initGLEW();
		void clearScreen();
		bool bindTexture(Texture* texture, Shader* shader, const char* samplerName, int textureUnit);
		void prepareRenderingConfiguration(Shader* shader);
		void revertRenderingConfiguration(Shader* shader);
		void bindDefaultRenderTarget();
		void unbindDefaultRenderTarget();

	public:
		Vector2i viewportSize;
		RenderDataList previousFrameRenderData;

	private:
		std::shared_ptr<FrameBuffer> m_defaultRenderTarget;
		std::unique_ptr<UniformBuffer> m_cameraUniformBuffer;
		std::unique_ptr<LightManager> m_lightManager; 
		RenderDataList m_renderDataList;
		SkyboxRenderData m_skyboxRenderData;
	};
}