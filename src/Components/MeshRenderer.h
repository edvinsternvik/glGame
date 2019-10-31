#pragma once
#include <memory>
#include "Component.h"
#include "../Model.h"

namespace glGame {
	
	class MeshRenderer : public Component {
	public:
		MeshRenderer();

		virtual std::string getName() const { return "Mesh Renderer"; }

		virtual void update() override;

	public:
		std::unique_ptr<Model> model;

	private:
		float m_tempTriangleVerticies[6] = {
			-0.5, -0.5, 0.5, -0.5, 0.0, 0.5
		}; 

	};

}