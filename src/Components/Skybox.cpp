#include "Skybox.h"
#include "../Application.h"

namespace glGame {

    Skybox::Skybox() : cubemap({"./Assets/skybox/px.png", "./Assets/skybox/nx.png", "./Assets/skybox/py.png", "./Assets/skybox/ny.png", "./Assets/skybox/pz.png", "./Assets/skybox/nz.png"}) {

    }

    void Skybox::init() {
    }

    void Skybox::onRender() {
        Application::Get().renderer.submit(&cubemap);
    }

}