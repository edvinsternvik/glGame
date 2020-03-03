#pragma once
#include "Component.h"

namespace glGame {

    class Script : public Component {
    public:
        Script();
        ~Script();

        virtual std::string getName() const { return "Script"; }

		virtual void update() override;

    public:
        const char* filename;

    };

}