#pragma once
#include "Component.h"

class asIScriptEngine;
class asIScriptFunction;

namespace glGame {

    class Script : public Component {
    public:
        Script();
        ~Script();

        virtual std::string getName() const { return "Script"; }

		virtual void update() override;

    public:
        const char* filename;

    private:
        asIScriptEngine* m_asScriptEngine;
        asIScriptFunction* m_asScriptUpdateFunction;
    };

}