#pragma once
#include "Component.h"
#include "PublicScriptVariable.h"
#include <unordered_map>

class asIScriptEngine;
class asIScriptObject;
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
        PublicVarVariant getPublicVarVariant(void* data, const PublicVariableType& type);

    private:
        std::string filename;
        asIScriptObject* m_scriptObj = nullptr;
        asIScriptContext* m_asScriptContext = nullptr;
        std::vector<PublicScriptVariable> m_scriptPublicVars;
        std::unordered_map<std::string, std::string> m_registeredPublicVars;

        static asIScriptEngine* s_asScriptEngine;
    };

}