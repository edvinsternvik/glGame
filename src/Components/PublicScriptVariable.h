#pragma once
#include <variant>
#include <string>
#include "../Math/Vector.h"
#include "PublicVariable.h"

namespace glGame {
    #define ScriptPublicVarVariant std::variant<int, float, double, char, std::string, Vector2, Vector3>

    class PublicScriptVariable {
    public:
        PublicScriptVariable(ScriptPublicVarVariant var, PublicVariableType datatype, std::string declaration);
        PublicScriptVariable(void* data, PublicVariableType datatype, std::string declaration);

    public:
        ScriptPublicVarVariant var;
        PublicVariableType type;
        std::string declaration;
    };
}