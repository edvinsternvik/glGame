#pragma once
#include "Component.h"
#include "../Rendering/Light.h"

namespace glGame {

    class LightComponent : public Component {
    public:
        LightComponent();

        virtual std::string getName() const { return "Light"; }

		virtual void init() override;
		virtual void onRender() override;

    public:
        Light light;
    };

}