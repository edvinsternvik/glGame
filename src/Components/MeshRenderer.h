#pragma once
#include <memory>
#include "RendererComponent.h"
#include <glm/glm.hpp>
#include "../Resources/Model.h"

namespace glGame {
	
	class MeshRenderer : public RendererComponent {
	public:
		MeshRenderer();

		virtual std::string getName() const { return "MeshRenderer"; }

		virtual void init() override;
		virtual void onRender() override;

		void changeModelFile(std::string str) { m_modelFilePath = str; }

	public:
		std::unique_ptr<Model> model;

	private:
		float m_tempTriangleVerticies[6] = {
			-0.5, -0.5, 0.5, -0.5, 0.0, 0.5
		};

		virtual void renderComponent(Shader* shader);

	private:
		std::string m_modelFilePath;
	};

}