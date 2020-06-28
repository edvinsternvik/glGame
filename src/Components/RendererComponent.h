#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace glGame {
    class Shader;

    class RendererComponent : public Component {
    public:
        virtual std::string getName() const = 0;

        virtual void init() override;

    protected:
        void updateModelMatrix();
        glm::mat4 modelMatrix;

    private:
        virtual void renderComponent(Shader* shader) = 0;
        friend class Renderer;
    };

}