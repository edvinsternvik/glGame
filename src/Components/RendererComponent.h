#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace glGame {
    class Shader;

    class RendererComponent : public Component {
    public:
        virtual std::string getName() const = 0;

    protected:
        void updateModelMatrix();
        glm::mat4 modelMatrix;
    };

}