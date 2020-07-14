#include "Skybox.h"
#include "../Application.h"

namespace glGame {

    Skybox::Skybox() {
        addPublicVariable(&textures[0], "PositiveX");
        addPublicVariable(&textures[1], "NegativeX");
        addPublicVariable(&textures[2], "PositiveY");
        addPublicVariable(&textures[3], "NegativeY");
        addPublicVariable(&textures[4], "PositiveZ");
        addPublicVariable(&textures[5], "NegativeZ");
    }

    void Skybox::init() {
        for(int i = 0; i < 6; ++i) {
            if(!textures[i].get()) return;
        }

        cubemap = std::make_unique<Cubemap>(textures);
    }

    void Skybox::onRender() {
        if(cubemap.get()) Application::Get().renderer.submit(cubemap.get());
    }

}