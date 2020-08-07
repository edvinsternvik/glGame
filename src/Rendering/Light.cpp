#include "Light.h"
#include "FrameBuffer.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>

namespace glGame {

    Light::Light(const LightType& lightType) : position(0.0, 0.0, 0.0), intensity(1.0), lightType(lightType) {

    }

    Light::Light(const LightType& lightType, const Vector3& position) : position(position), intensity(1.0), lightType(lightType) {

    }

    Light::Light(const LightType& lightType, const Vector3& position, const float& intensity) : position(position), intensity(intensity), lightType(lightType) {

    }

    Light::~Light() {

    }

    void Light::updateLight() {

        if(castShadows && lightType == LightType::Directional) {
            glm::mat4 lightProjection = glm::ortho(-shadowSize.x, shadowSize.x, -shadowSize.y, shadowSize.y, shadowNearPlane, shadowFarPlane);
            glm::vec3 pos(position.x, position.y, position.z);
            glm::vec3 dir(direction.x, direction.y, direction.z);
            glm::mat4 lightView = glm::lookAt(pos, dir, glm::vec3( 0.0f, 1.0f,  0.0f)); 
            m_lightSpaceMatrix = lightProjection * lightView;

            if(!m_shadowmapFramebuffer.get()) {
                std::shared_ptr<Texture> framebufferTexture = std::make_shared<Texture>(shadowmapSize.x, shadowmapSize.y, TextureType::DEPTH);
		        m_shadowmapFramebuffer = std::make_unique<FrameBuffer>(framebufferTexture);
            }
        }
        else if(m_shadowmapFramebuffer.get()) {
            m_shadowmapFramebuffer = std::unique_ptr<FrameBuffer>();
        }
    }
}