#include "PublicScriptVariable.h"

namespace glGame {

    PublicScriptVariable::PublicScriptVariable(ScriptPublicVarVariant var, PublicVariableType datatype, std::string declaration) 
    : var(var), type(datatype), declaration(declaration) {  

    }

    PublicScriptVariable::PublicScriptVariable(void* data, PublicVariableType datatype, std::string declaration) 
    : type(datatype), declaration(declaration) {

        switch(datatype) {
		case PublicVariableType::Int: var = *(int*)data; break;
		case PublicVariableType::Float: var = *(float*)data; break;
		case PublicVariableType::Double: var = *(double*)data; break;
		case PublicVariableType::Vec2: var = *(Vector2*)data; break;
		case PublicVariableType::Vec3: var = *(Vector3*)data; break;
		case PublicVariableType::Char: var = *(char*)data; break;
		case PublicVariableType::String: var = *(std::string*)data; break;
		// case PublicVariableType::GameObject:
		// case PublicVariableType::Component:
		// case PublicVariableType::Asset:
		// case PublicVariableType::Color:
		}
    }
}