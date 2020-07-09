#pragma once
#include "RendererComponent.h"
#include <glm/glm.hpp>
#include <memory>
#include "../Math/Vector.h"
#include "../Rendering/VertexArray.h"
#include "../Rendering/VertexBuffer.h"

namespace glGame {
	
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
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<VertexBuffer> m_vbo;
	};

}