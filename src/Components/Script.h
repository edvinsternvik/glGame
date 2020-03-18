#pragma once
#include "Component.h"
#include "../Math/Vector.h"
#include <variant>

#define ScriptPublicVarVariant std::variant<int, float, double, char, std::string, Vector2, Vector3>
#define ScriptPublicVarType std::pair<ScriptPublicVarVariant, std::string>

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
        std::vector<ScriptPublicVarType> m_scriptPublicVars;
        std::vector<ScriptPublicVarType> m_registeredPVars;
    };

}