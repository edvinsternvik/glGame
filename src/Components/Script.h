#pragma once
#include "Component.h"
#include "PublicScriptVariable.h"
#include <unordered_map>

class asIScriptEngine;
class asIScriptFunction;
class asIScriptContext;

namespace glGame {
    
    class Script : public Component {
    public:
        Script();
        ~Script();

        virtual std::string getName() const { return "Script"; }

        virtual void init() override;
		virtual void update(float deltatime) override;

        void changeScriptfile(std::string& scriptfile) { filename = scriptfile; }

        void registerPublicScriptVariable(std::vector<std::string>& strings);

    private:
        void cleanupScriptEngine();

    private:
        std::string filename;

        asIScriptEngine* m_asScriptEngine = nullptr;
        asIScriptFunction* m_asScriptInitFunction;
        asIScriptFunction* m_asScriptUpdateFunction;
        asIScriptContext* m_asScriptContext = nullptr;
        std::vector<PublicScriptVariable> m_scriptPublicVars;
        std::unordered_map<std::string, std::string> m_registeredPublicVars;
    };

}