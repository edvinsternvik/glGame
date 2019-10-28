#pragma once
#include <memory>
#include "Component.h"

namespace glGame {

	// class Model;
	
	class MeshRenderer : public Component {
	public:
		// std::unique_ptr<Model> model;

		virtual std::string getName() const { return "Mesh Renderer"; }

		virtual void update() override;

	public:

	};

}