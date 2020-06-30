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

	public:
		ModelAsset model;

	private:

		virtual void renderComponent(Shader* shader);
	};

}