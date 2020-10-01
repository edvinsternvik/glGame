#pragma once
#include "../Math/Vector.h"
#include "../Rendering/Texture.h"

#include <memory>

namespace glGame {

    class Editor;
    class FrameBuffer;
	class Shader;
    class MeshRenderer;
    class GameObject;
    class Model;

    class EditorRenderer {
    public:
        EditorRenderer(const Vector2i& viewportSize, Editor* editor);
        ~EditorRenderer();

        void renderEditorGizmos();
        std::shared_ptr<Texture> getFrameTexture();

    private:
        Editor* m_editor;
        std::shared_ptr<FrameBuffer> m_viewportFrameBuffer;
		std::unique_ptr<Shader> m_objectOutlineStencilWriteShader;
        std::unique_ptr<Shader> m_objectOutlineShader;
        std::weak_ptr<MeshRenderer> m_selectedMeshRenderer;

        std::unique_ptr<Model> m_transformMoveArrowModel;
        std::unique_ptr<Shader> m_transformGizmoShader;
    };

}