#pragma once
#include <GL/glew.h>
#include "FrameBuffer.h"
#include "Shader.h"
#include "Model.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "UniformBuffer.h"

namespace glGame {

	class Scene;
	class GameObject;
	class Camera;
	class Material;
	class Cubemap;
	class Light;

	class Renderer {
	public:
		void init();

		void submit(Model* model, const glm::mat4& modelMatrix);
		void submit(VertexArray* vertexArray, const unsigned int& verticies, const glm::mat4& modelMatrix);
		void submit(Cubemap* cubemap, Shader* shader);
		unsigned int addLight(const Light& light);
		void updateLight(const unsigned int& lightId, const Light& light);
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

		void initGLEW();
		void processRenderData(std::vector<ObjectRenderData>& frameRenderData);
		void clearScreen();

	private:
		std::unique_ptr<FrameBuffer> m_editorFramebuffer;
		std::unique_ptr<UniformBuffer> m_cameraUniformBuffer;
		std::unique_ptr<UniformBuffer> m_lightsUniformBuffer;
		ObjectRenderData m_objectRenderData;
		SkyboxRenderData m_skyboxRenderData;
		unsigned int m_lightCount = 0, m_lightIdCount = 0, m_lightCountOffset;
		std::unordered_map<unsigned int, unsigned int> m_lightOffsets;

	};
}