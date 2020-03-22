#include "PublicVariable.h"
#include "../Math/Vector.h"
#include <sstream>

namespace glGame {

    PublicVariable::PublicVariable(void* data, PublicVariableType variableType, std::string name) 
    : data(data), variableType(variableType), name(name), editor_sliderSpeed(1.0f) {

    }

    PublicVariable::PublicVariable(void* data, PublicVariableType variableType, std::string name, float sliderSpeed) 
    : data(data), variableType(variableType), name(name), editor_sliderSpeed(sliderSpeed) {

    }

    void PublicVariable::setData(std::string& str) {
        switch(variableType) {
		case PublicVariableType::Int: *(int*)data = std::stoi(str); return;
		case PublicVariableType::Float: *(float*)data = std::stof(str); return;
		case PublicVariableType::Double: *(double*)data = std::stod(str); return;
		case PublicVariableType::Vec2: *(Vector2*)data = Vector2(str); return;
		case PublicVariableType::Vec3:	 *(Vector3*)data = Vector3(str); return;
		case PublicVariableType::Char: *(char*)data = str[0]; return;
		case PublicVariableType::String: *(std::string*)data = str; return;
		// case PublicVariableType::GameObject:
		// case PublicVariableType::Component:
		// case PublicVariableType::Asset:
		// case PublicVariableType::Color:
		}
        
    }

    PublicVariableType PublicVariable::getPublicVariableType(std::string& str) {
             if(str == "int") return PublicVariableType::Int;
        else if(str == "float") return PublicVariableType::Float;
        else if(str == "double") return PublicVariableType::Double;
        else if(str == "Vector2") return PublicVariableType::Vec2;
        else if(str == "Vector3") return PublicVariableType::Vec3;
        else if(str == "char") return PublicVariableType::Char;
        else if(str == "string") return PublicVariableType::String;
        else if(str == "GameObject") return PublicVariableType::GameObject;
        else if(str == "Component") return PublicVariableType::Component;
        else if(str == "Asset") return PublicVariableType::Asset;
        else if(str == "Color") return PublicVariableType::Color;

        return PublicVariableType::None;
    }

}