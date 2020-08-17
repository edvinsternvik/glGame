#include "EditorRenderer.h"
#include "Editor.h"
#include "../Application.h"
#include "../Rendering/FrameBuffer.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"
#include "../Components/MeshRenderer.h"
#include "../Components/Transform.h"

namespace glGame {

    EditorRenderer::EditorRenderer(const Vector2i& viewportSize, Editor* editor)
        : m_viewportFrameBuffer(std::make_shared<FrameBuffer>(viewportSize.x, viewportSize.y)), m_editor(editor) {
        Application::Get().renderer.setDefaultRenderTarget(m_viewportFrameBuffer);
        
        m_objectOutlineStencilWriteShader = std::make_unique<Shader>(nullptr, 
            "#section color\n"
            "Disable\n"
            "\n"
            "#section stencil\n"
            "Enable\n"
            "StencilFunc Always\n"
            "StencilOpDPPass Replace\n"
            "\n"
            "#section depthtest\n"
            "Disable\n"
            "\n"
			"#section vertex\n"
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (std140) uniform Camera {\n"
			"	mat4 u_projection;\n"
			"	mat4 u_view;\n"
			"};\n"
            "uniform mat4 u_model;\n"
            "void main() { gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0); }\n"
            "\n"
            "#section fragment\n"
            "#version 330 core\n"
            "void main() {}\n"
		);

        m_objectOutlineShader = std::make_unique<Shader>(nullptr, 
            "#section stencil\n"
            "Enable\n"
            "StencilFunc NotEqual\n"
            "\n"
			"#section vertex\n"
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (std140) uniform Camera {\n"
			"	mat4 u_projection;\n"
			"	mat4 u_view;\n"
			"};\n"
            "uniform mat4 u_model;\n"
            "uniform vec3 u_scale;\n"
            "void main() {\n"
            "   vec3 scaledPos = aPos * u_scale;\n"
            "   float distSquare = scaledPos.x * scaledPos.x + scaledPos.y * scaledPos.y + scaledPos.z * scaledPos.z;\n"
            "   float scale = 0.0;\n"
            "   if(distSquare > 0.0) scale = sqrt(0.0025 / distSquare);\n"
            "   float dist = (u_projection * u_view * u_model * vec4(aPos, 1.0)).w;\n"
            "   if(dist <= 0) dist = 0.001;\n" 
            "   scale *= sqrt(dist);\n"
            "   gl_Position = u_projection * u_view * u_model * vec4(aPos * (1.0 + scale), 1.0); }\n"
            "\n"
            "#section fragment\n"
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {FragColor = vec4(0.9, 0.0, 0.0, 1.0);}\n"
		);
    }

    EditorRenderer::~EditorRenderer() {

    }

    void EditorRenderer::renderEditorGizmos() {
        // Render object outline for selected object
        std::weak_ptr<GameObject> selectedObj = m_editor->getSelectedItem<GameObject>();
        if(selectedObj.expired()) {
            m_selectedMeshRenderer = std::weak_ptr<MeshRenderer>();
        }
        else if(m_selectedMeshRenderer.expired() || m_selectedMeshRenderer.lock()->getGameObject() != selectedObj.lock().get()) {
            m_selectedMeshRenderer = selectedObj.lock()->getComponent<MeshRenderer>();
        }
        
        MeshRenderer* meshRenderer = m_selectedMeshRenderer.lock().get();
        if(meshRenderer && !meshRenderer->model.expired()) {
            glm::mat4 transformMatrix = meshRenderer->getGameObject()->transform->getTransformMatrix();
            Vector3 scale = meshRenderer->getGameObject()->transform->scale;

            m_objectOutlineShader->useShader();
            m_objectOutlineShader->setUniform3f("u_scale", scale.x, scale.y, scale.z);
            Application::Get().renderer.submit(meshRenderer->model.get(), transformMatrix, m_objectOutlineStencilWriteShader.get());
            Application::Get().renderer.submit(meshRenderer->model.get(), transformMatrix, m_objectOutlineShader.get());
        }
    }

    std::shared_ptr<Texture> EditorRenderer::getFrameTexture() {
        return m_viewportFrameBuffer->getTexture();
    }



}