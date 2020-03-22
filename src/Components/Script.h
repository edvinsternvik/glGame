#pragma once
#include "Component.h"
#include "PublicScriptVariable.h"
#include <unordered_map>

class asIScriptEngine;
class asIScriptFunction;

namespace glGame {
    
    class Script : public Component {
    public:
        Script();

        virtual std::string getName() const { return "Script"; }

        virtual void init() override;
		virtual void update() override;

        void registerPublicScriptVariable(std::vector<std::string>& strings);

    private:
        std::string filename;

        asIScriptEngine* m_asScriptEngine;
        asIScriptFunction* m_asScriptInitFunction;
        std::vector<PublicScriptVariable> m_scriptPublicVars;
        std::unordered_map<std::string, std::string> m_registeredPublicVars;
    };

}