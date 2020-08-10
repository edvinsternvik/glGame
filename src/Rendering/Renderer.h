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
	class LightManager;

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

	class Renderer {
	public:
		Renderer();
		~Renderer();

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
		std::vector<ObjectRenderData> frameRenderData;

	private:
		std::unique_ptr<FrameBuffer> m_editorFramebuffer;
		std::unique_ptr<UniformBuffer> m_cameraUniformBuffer;
		std::unique_ptr<LightManager> m_lightManager;
		ObjectRenderData m_objectRenderData;
		SkyboxRenderData m_skyboxRenderData;
	};
}