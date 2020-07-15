#pragma once
#include "Component.h"
#include "../Rendering/Cubemap.h"
#include "../Rendering/Shader.h"
#include "../Resources/Asset.h"

namespace glGame {

    class Skybox : public Component {
    public:
        Skybox();

        virtual std::string getName() const { return "Skybox"; }

		virtual void init() override;
		virtual void onRender() override;

    public:
        std::unique_ptr<Cubemap> cubemap;
        Asset<Shader> shader;
        Asset<Texture> textures[6];
    };

}