#include "PublicVariable.h"
#include "../Math/Vector.h"
#include "../Resources/AssetManager.h"
#include "../Resources/ModelAsset.h"
#include "../Resources/ScriptAsset.h"
#include "../Resources/TextureAsset.h"
#include "../Resources/ShaderAsset.h"
#include <sstream>

namespace glGame {

    PublicVariableEnum::PublicVariableEnum() : selection(0) {
    }

    PublicVariableEnum::PublicVariableEnum(int selection, const std::vector<const char*>& options) {
        for(int i = 0; i < options.size(); ++i) {
            int stringlength = std::strlen(options[i]);
            this->options.push_back(new char[stringlength + 1]);
            std::strcpy(this->options[i], options[i]);
            this->options[i][stringlength] = '\0';
        }
    }

    PublicVariableEnum::PublicVariableEnum(const PublicVariableEnum& other) {
        selection = other.selection;
    }

    PublicVariableEnum::~PublicVariableEnum() {
        if(options.size() > 0) {
            for(int i = 0; i < options.size(); ++i) delete options[i];
            options.clear();
        }
    }

    PublicVariableEnum& PublicVariableEnum::operator=(const PublicVariableEnum& other)  {
        selection = other.selection;
        return *this;
    }

    PublicVariable::PublicVariable(PublicVarVariant data, std::string name)
    : data(data), name(name), editor_sliderSpeed(1.0f) {

    }

    PublicVariable::PublicVariable(PublicVarVariant data, std::string name, const float& sliderSpeed)
    : data(data), name(name), editor_sliderSpeed(sliderSpeed) {

    }

    void PublicVariable::setData(std::string& str) {
        switch(data.index()) {
		case toInt(PublicVariableType::Int): *std::get<int*>(data) = std::stoi(str); return;
		case toInt(PublicVariableType::Float): *std::get<float*>(data) = std::stof(str); return;
		case toInt(PublicVariableType::Double): *std::get<double*>(data) = std::stod(str); return;
		case toInt(PublicVariableType::Char): *std::get<char*>(data) = str[0]; return;
		case toInt(PublicVariableType::String): *std::get<std::string*>(data) = str; return;
		case toInt(PublicVariableType::Vec2): *std::get<Vector2*>(data) = Vector2(str); return;
		case toInt(PublicVariableType::Vec3): *std::get<Vector3*>(data) = Vector3(str); return;
		// case PublicVariableType::GameObject:
		// case PublicVariableType::Component:
		// case PublicVariableType::Color:
        case toInt(PublicVariableType::PublicVariableEnum): std::get<PublicVariableEnum*>(data)->selection = std::stoi(str); return;
        case toInt(PublicVariableType::Model): *std::get<Asset<Model>*>(data) = AssetManager::Get().getAsset<Model>(str); return;
        case toInt(PublicVariableType::Script): *std::get<Asset<Script>*>(data) = AssetManager::Get().getAsset<Script>(str); return;
        case toInt(PublicVariableType::Texture): *std::get<Asset<Texture>*>(data) = AssetManager::Get().getAsset<Texture>(str); return;
        case toInt(PublicVariableType::Shader): *std::get<Asset<Shader>*>(data) = AssetManager::Get().getAsset<Shader>(str); return;
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
        else if(str == "Color") return PublicVariableType::Color;
        else if(str == "Model") return PublicVariableType::Model;
        else if(str == "Script") return PublicVariableType::Script;
        else if(str == "Texture") return PublicVariableType::Texture;
        else if(str == "Shader") return PublicVariableType::Shader;

        return PublicVariableType::None;
    }

}