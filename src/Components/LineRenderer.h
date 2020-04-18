#pragma once
#include <memory>
#include "RendererComponent.h"
#include <glm/glm.hpp>
#include <memory>
#include "../Math/Vector.h"

namespace glGame {

    class VertexArray;
    class VertexBuffer;
	
	class LineRenderer : public RendererComponent {
	public:
		LineRenderer();

		virtual std::string getName() const { return "LineRenderer"; }

		virtual void init() override;
		virtual void onRender() override;

	public:
        Vector3 lineLength;
        float lineWidth = 5.0;

	private:
		virtual void renderComponent(Shader* shader);

    private:
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<VertexBuffer> m_vbo;
	};

}