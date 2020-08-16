#include "PublicVariable.h"
#include "../Math/Vector.h"
#include "../Resources/AssetManager.h"
#include "../Resources/ModelAsset.h"
#include "../Resources/ScriptAsset.h"
#include "../Resources/TextureAsset.h"
#include "../Resources/ShaderAsset.h"
#include "../Resources/MaterialAsset.h"
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

    template<typename T>
    internal::Vector2<T> createVector2FromString(std::string& str, T (*strToType)(const std::string& s)) {
        internal::Vector2<T> vec2(0, 0);

        std::stringstream valueBuffer;
        valueBuffer << str.substr(1, str.size() - 2);
        std::string tmp;
        for(int i = 0; i < 2; ++i) {
            std::getline(valueBuffer, tmp, ',');
            
            ((T*)&vec2)[i] = strToType(tmp);
        };

        return vec2;
    }

    template<typename T>
    internal::Vector3<T> createVector3FromString(std::string& str, T (*strToType)(const std::string& s)) {
        internal::Vector3<T> vec3(0, 0, 0);

        std::stringstream valueBuffer;
        valueBuffer << str.substr(1, str.size() - 2);
        std::string tmp;
        for(int i = 0; i < 3; ++i) {
            std::getline(valueBuffer, tmp, ',');
            
            ((T*)&vec3)[i] = strToType(tmp);
        };

        return vec3;
    }

    float stofOverload(const std::string& str) { return std::stof(str); }
    int stoiOverload(const std::string& str) { return std::stoi(str); }

    void PublicVariable::setData(std::string& str) {
        switch(data.index()) {
		case toInt(PublicVariableType::Int): *std::get<int*>(data) = std::stoi(str); return;
		case toInt(PublicVariableType::Float): *std::get<float*>(data) = std::stof(str); return;
		case toInt(PublicVariableType::Double): *std::get<double*>(data) = std::stod(str); return;
		case toInt(PublicVariableType::Char): *std::get<char*>(data) = str[0]; return;
		case toInt(PublicVariableType::String): *std::get<std::string*>(data) = str; return;
        case toInt(PublicVariableType::Boolean): *std::get<bool*>(data) = (bool)std::stoi(str); return;
		case toInt(PublicVariableType::Vec2): *std::get<Vector2*>(data) = createVector2FromString<float>(str, stofOverload); return;
        case toInt(PublicVariableType::Vec2i): *std::get<Vector2i*>(data) = createVector2FromString<int>(str, stoiOverload); return;
		case toInt(PublicVariableType::Vec3): *std::get<Vector3*>(data) = createVector3FromString<float>(str, stofOverload); return;
		case toInt(PublicVariableType::Vec3i): *std::get<Vector3i*>(data) = createVector3FromString<int>(str, stoiOverload); return;
		// case PublicVariableType::GameObject:
		// case PublicVariableType::Component:
		// case PublicVariableType::Color:
        case toInt(PublicVariableType::PublicVariableEnum): std::get<PublicVariableEnum*>(data)->selection = std::stoi(str); return;
        case toInt(PublicVariableType::Model): *std::get<Asset<Model>*>(data) = AssetManager::Get().getAsset<Model>(str); return;
        case toInt(PublicVariableType::Script): *std::get<Asset<Script>*>(data) = AssetManager::Get().getAsset<Script>(str); return;
        case toInt(PublicVariableType::Texture): *std::get<Asset<Texture>*>(data) = AssetManager::Get().getAsset<Texture>(str); return;
        case toInt(PublicVariableType::Shader): *std::get<Asset<Shader>*>(data) = AssetManager::Get().getAsset<Shader>(str); return;
        case toInt(PublicVariableType::Material): *std::get<Asset<Material>*>(data) = AssetManager::Get().getAsset<Material>(str); return;
		}
        
    }

    template<typename T>
    std::string createStringFromVector2(const internal::Vector2<T>& vec) {
        return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "}";
    }

    template<typename T>
    std::string createStringFromVector3(const internal::Vector3<T>& vec) {
        return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "," + std::to_string(vec.z) + "}";
    }

    std::string PublicVariable::getPublicVariableString() {
		switch(data.index()) {
		case toInt(PublicVariableType::Int): return std::to_string(*(std::get<int*>(data)));
		case toInt(PublicVariableType::Float): return std::to_string(*(std::get<float*>(data)));
		case toInt(PublicVariableType::Double): return std::to_string(*(std::get<double*>(data)));
		case toInt(PublicVariableType::Char): return std::to_string(*(std::get<char*>(data)));
		case toInt(PublicVariableType::String): return *(std::get<std::string*>(data));
        case toInt(PublicVariableType::Boolean): return std::to_string(*(std::get<bool*>(data)));
		case toInt(PublicVariableType::Vec2): return createStringFromVector2(*(std::get<Vector2*>(data)));
        case toInt(PublicVariableType::Vec2i): return createStringFromVector2(*(std::get<Vector2i*>(data)));
        case toInt(PublicVariableType::Vec3): return createStringFromVector3(*(std::get<Vector3*>(data)));
		case toInt(PublicVariableType::Vec3i): return createStringFromVector3(*(std::get<Vector3i*>(data)));
		case toInt(PublicVariableType::PublicVariableEnum): return std::to_string(std::get<PublicVariableEnum*>(data)->selection);
		// case PublicVariableType::GameObject: return "";
		// case PublicVariableType::Component: return "";
		// case PublicVariableType::Color: return "";
		case toInt(PublicVariableType::Model): return (*std::get<Asset<Model>*>(data)).getPath();
		case toInt(PublicVariableType::Script): return (*std::get<Asset<Script>*>(data)).getPath();
		case toInt(PublicVariableType::Texture): return (*std::get<Asset<Texture>*>(data)).getPath();
		case toInt(PublicVariableType::Shader): return (*std::get<Asset<Shader>*>(data)).getPath();
        case toInt(PublicVariableType::Material): return (*std::get<Asset<Material>*>(data)).getPath();
		}
		return "";
	}

    PublicVariableType PublicVariable::getPublicVariableType(std::string& str) {
             if(str == "int") return PublicVariableType::Int;
        else if(str == "float") return PublicVariableType::Float;
        else if(str == "double") return PublicVariableType::Double;
        else if(str == "Vector2") return PublicVariableType::Vec2;
        else if(str == "Vector2i") return PublicVariableType::Vec2i;
        else if(str == "Vector3") return PublicVariableType::Vec3;
        else if(str == "Vector3i") return PublicVariableType::Vec3i;
        else if(str == "char") return PublicVariableType::Char;
        else if(str == "string") return PublicVariableType::String;
        else if(str == "boolean") return PublicVariableType::Boolean;
        else if(str == "GameObject") return PublicVariableType::GameObject;
        else if(str == "Component") return PublicVariableType::Component;
        else if(str == "Color") return PublicVariableType::Color;
        else if(str == "Model") return PublicVariableType::Model;
        else if(str == "Script") return PublicVariableType::Script;
        else if(str == "Texture") return PublicVariableType::Texture;
        else if(str == "Shader") return PublicVariableType::Shader;
        else if(str == "Material") return PublicVariableType::Material;

        return PublicVariableType::None;
    }

}