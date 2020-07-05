#pragma once
#include "Component.h"
#include "../Script/PublicScriptVariable.h"
#include "../Script/Script.h"

#include <unordered_map>
#include <memory>

class asIScriptObject;

namespace glGame {

    class ScriptComponent : public Component {
    public:
        ~ScriptComponent();

        virtual std::string getName() const { return "Script"; }

        virtual void init() override;
		virtual void update(float deltatime) override;

        void registerPublicScriptVariable(std::vector<std::string>& strings);

    private:
        PublicVarVariant getPublicVarVariant(void* data, const PublicVariableType& type);
        void cleanupScript();

    private:
        std::shared_ptr<Script> m_script;
        asIScriptObject* m_scriptObj = nullptr;

        std::vector<PublicScriptVariable> m_scriptPublicVars;
        std::unordered_map<std::string, std::string> m_registeredPublicVars;

    };

}