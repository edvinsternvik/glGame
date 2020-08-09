#pragma once
#include "FrameBuffer.h"
#include "Shader.h"
#include "Model.h"
#include "UniformBuffer.h"
#include "../Math/Vector.h"
#include <GL/glew.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace glGame {

	class Scene;
	class GameObject;
	class Camera;
	class Material;
	class Cubemap;
	class Light;

	class Renderer {
	public:
		void init(Vector2i viewportSize);

		void submit(Model* model, const glm::mat4& modelMatrix);
		void submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix);
		void submit(Cubemap* cubemap, Shader* shader);
		void updateLight(std::shared_ptr<Light> light);
		void deleteLight(const unsigned int& lightid);
		void setMaterial(Material* material);

		void beginRender();
		void render(Scene* scene);
		void renderGizmos(const std::vector<GameObject*>& gizmoObjects);
		void endRender();

		unsigned int getEditorFrameTexture();

	private:
		struct ObjectRenderData {
		public:
			ObjectRenderData() {}
			ObjectRenderData(VertexArray* vao, const unsigned int& verticies, Material* material, const glm::mat4& modelMatrix)
				: vao(vao), verticies(verticies), material(material), modelMatrix(modelMatrix) {}

			VertexArray* vao;
			unsigned int verticies;
			Material* material;
			glm::mat4 modelMatrix;
		};

		struct SkyboxRenderData {
			SkyboxRenderData() : cubemap(nullptr), shader(nullptr) {}
			SkyboxRenderData(Cubemap* cubemap, Shader* shader) : cubemap(cubemap), shader(shader) {}
			Cubemap* cubemap;
			Shader* shader;
		};

		void renderObjects(std::vector<ObjectRenderData>& renderData);
		void renderObjectsShadow(std::vector<ObjectRenderData>& renderData);
		void initGLEW();
		void processRenderData(std::vector<ObjectRenderData>& frameRenderData);
		void clearScreen();

	public:
		Vector2i viewportSize;

	private:
		static const unsigned int MAX_SHADOWMAPS = 8;

		std::unique_ptr<FrameBuffer> m_editorFramebuffer;
		std::unique_ptr<Shader> m_shadowmapShader;
		std::shared_ptr<TextureArray> m_shadowmapTextureArray;
		std::unique_ptr<UniformBuffer> m_cameraUniformBuffer;
		std::unique_ptr<UniformBuffer> m_lightsUniformBuffer;
		ObjectRenderData m_objectRenderData;
		SkyboxRenderData m_skyboxRenderData;
		unsigned int m_lightCountOffset;
		std::vector<std::shared_ptr<Light>> m_lights;
		unsigned int m_shadowmapCount = 0;
		std::array<bool, MAX_SHADOWMAPS> m_shadowmaps;
	};
}