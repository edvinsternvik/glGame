#include "ObjectPicker.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Renderer.h"

#include <GL/glew.h>

namespace glGame {

    Vector3i calculateColorFromId(unsigned int id);

    ObjectPicker::ObjectPicker(Vector2i resolution) : m_resolution(resolution) {
        m_colorPickerShader = std::make_unique<Shader>(nullptr, 
			"#type vertex\n"
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 u_projectionView;\n"
            "uniform mat4 u_model;\n"
            "void main() { gl_Position = u_projectionView * u_model * vec4(aPos, 1.0); }\n"
            "\n"
            "#type fragment\n"
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 u_uniqueColor;\n"
            "void main() {FragColor = vec4(u_uniqueColor, 1.0);}\n"
		);

        m_colorPickerFramebuffer = std::make_unique<FrameBuffer>(resolution.x, resolution.y);
        m_colorPickerTexture = m_colorPickerFramebuffer->getTexture();
    }

    ObjectPicker::~ObjectPicker() {

    }

    int ObjectPicker::getGameObjectIdFromScreen(const Vector2& normalizedScreenPosition) {
        if(normalizedScreenPosition.x < 0.0 || normalizedScreenPosition.x > 1.0 || normalizedScreenPosition.y < 0.0 || normalizedScreenPosition.y > 1.0) return -1;

        m_colorPickerFramebuffer->bind();
        GLubyte pixel[4];
        Vector2i screenPosition(normalizedScreenPosition.x * m_resolution.x, (1.0 - normalizedScreenPosition.y) * m_resolution.y);
        glReadPixels(screenPosition.x, screenPosition.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

        if((int)pixel[0] == 255 && (int)pixel[1] == 255 && (int)pixel[2] == 255) return -1;

        return (int)pixel[2] + (((int)pixel[1]) << 8) + (((int)pixel[0]) << 16);
    }

    void ObjectPicker::renderColorPickerTexture(std::vector<ObjectRenderData>* objRenderData, Camera* camera) {
        glm::mat4 projectionView = camera->getProjectionMatrix() * camera->getViewMatrix();

        m_colorPickerShader->useShader();
        m_colorPickerShader->setUniformMat4("u_projectionView", &(projectionView[0][0]));

        glViewport(0, 0, m_resolution.x, m_resolution.y);

        GLfloat clearColor[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        m_colorPickerFramebuffer->bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(auto obj : *objRenderData) {
            obj.vao->bind();
            m_colorPickerShader->setUniformMat4("u_model", &(obj.modelMatrix[0][0]));

            Vector3i uniqueColor = calculateColorFromId(obj.gameObjectId);
            m_colorPickerShader->setUniform3f("u_uniqueColor", uniqueColor.x / 255.0, uniqueColor.y / 255.0, uniqueColor.z / 255.0);
            glDrawArrays(GL_TRIANGLES, 0, obj.verticies);
        }

        m_colorPickerFramebuffer->unbind();
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    }

    Vector3i calculateColorFromId(unsigned int id) {
        Vector3i result(0, 0, 0);
        result.z = id & 0xFF;
        result.y = (id >> 8) & 0xFF;
        result.x = (id >> 16) & 0xFF;
        return result;
    }

}