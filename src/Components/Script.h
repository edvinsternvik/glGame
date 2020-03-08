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

        virtual void init() override;
		virtual void update() override;

    private:
        std::string filename;

    private:
        asIScriptEngine* m_asScriptEngine;
        asIScriptFunction* m_asScriptInitFunction;
    };

}