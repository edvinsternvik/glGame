#pragma once
#include "Component.h"
#include "../Script/PublicScriptVariable.h"
#include "../Resources/ScriptAsset.h"

#include <unordered_map>
#include <memory>

class asIScriptObject;

namespace glGame {

    class ScriptComponent : public Component {
    public:
        ScriptComponent();
        ~ScriptComponent();

        virtual std::string getName() const { return "Script"; }

        virtual void init() override;
		virtual void update(float deltatime) override;

        void registerPublicScriptVariable(std::vector<std::string>& strings);

    private:
        PublicVarVariant getPublicVarVariant(void* data, const PublicVariableType& type);
        void cleanupScript();

    public:
        Asset<Script> script;

    private:
        asIScriptObject* m_scriptObj = nullptr;

        std::vector<PublicScriptVariable> m_scriptPublicVars;
        std::unordered_map<std::string, std::string> m_registeredPublicVars;

    };

}