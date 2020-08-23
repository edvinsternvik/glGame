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

    template<typename T, typename U, int size>
    T createTypeFromString(std::string& str, U (*strToType)(const std::string& s)) {
        T type;

        std::stringstream valueBuffer;
        valueBuffer << str.substr(1, str.size() - 2);
        std::string tmp;
        for(int i = 0; i < size; ++i) {
            std::getline(valueBuffer, tmp, ',');
            
            type[i] = strToType(tmp);
        };

        return type;
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
		case toInt(PublicVariableType::Vec2): *std::get<Vector2*>(data) = createTypeFromString<Vector2, float, 2>(str, stofOverload); return;
        case toInt(PublicVariableType::Vec2i): *std::get<Vector2i*>(data) = createTypeFromString<Vector2i, int, 2>(str, stoiOverload); return;
		case toInt(PublicVariableType::Vec3): *std::get<Vector3*>(data) = createTypeFromString<Vector3, float, 3>(str, stofOverload); return;
		case toInt(PublicVariableType::Vec3i): *std::get<Vector3i*>(data) = createTypeFromString<Vector3, int, 3>(str, stoiOverload); return;
        case toInt(PublicVariableType::Quaternion): *std::get<Quaternion*>(data) = createTypeFromString<Quaternion, float, 4>(str, stofOverload); return;
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

    template<typename T, int size>
    std::string createStringFromType(const T& type) {
        std::stringstream ss;
        ss << "{";
        for(int i = 0; i < size; ++i) {
            if(i > 0) ss << ",";
            ss << type[i];
        }
        ss << "}";
        return ss.str();
    }

    std::string PublicVariable::getPublicVariableString() {
		switch(data.index()) {
		case toInt(PublicVariableType::Int): return std::to_string(*(std::get<int*>(data)));
		case toInt(PublicVariableType::Float): return std::to_string(*(std::get<float*>(data)));
		case toInt(PublicVariableType::Double): return std::to_string(*(std::get<double*>(data)));
		case toInt(PublicVariableType::Char): return std::to_string(*(std::get<char*>(data)));
		case toInt(PublicVariableType::String): return *(std::get<std::string*>(data));
        case toInt(PublicVariableType::Boolean): return std::to_string(*(std::get<bool*>(data)));
		case toInt(PublicVariableType::Vec2): return createStringFromType<Vector2, 2>(*(std::get<Vector2*>(data)));
        case toInt(PublicVariableType::Vec2i): return createStringFromType<Vector2i, 2>(*(std::get<Vector2i*>(data)));
        case toInt(PublicVariableType::Vec3): return createStringFromType<Vector3, 3>(*(std::get<Vector3*>(data)));
		case toInt(PublicVariableType::Vec3i): return createStringFromType<Vector3i, 3>(*(std::get<Vector3i*>(data)));
        case toInt(PublicVariableType::Quaternion): return createStringFromType<Quaternion, 4>(*(std::get<Quaternion*>(data)));
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
        else if(str == "Quaternion") return PublicVariableType::Quaternion;
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