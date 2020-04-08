#pragma once
#include <memory>
#include "Component.h"
#include <glm/glm.hpp>
#include "../Resources/Model.h"

namespace glGame {
	
	class MeshRenderer : public Component {
	public:
		MeshRenderer();

		virtual std::string getName() const { return "MeshRenderer"; }

		virtual void init() override;
		virtual void onRender() override;

		void changeModelFile(std::string str) { m_modelFilePath = str; }

	public:
		std::unique_ptr<Model> model;
		glm::mat4 modelMatrix;

	private:
		float m_tempTriangleVerticies[6] = {
			-0.5, -0.5, 0.5, -0.5, 0.0, 0.5
		}; 

	private:
		void updateModelMatrix();
		std::string m_modelFilePath;
	};

}