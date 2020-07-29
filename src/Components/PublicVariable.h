#pragma once
#include "../Math/Vector.h"
#include <string>
#include <variant>
#include <memory>
#include <cstring>
#include <vector>

namespace glGame {
	
	class GameObject;
	class Component;
	class Color;

	template<class T>
	class Asset;
	class Model;
    class Texture;
    class Script;
    class Shader;

	class PublicVariableEnum {
	public:
		PublicVariableEnum();
		PublicVariableEnum(int selection, const std::vector<const char*>& options);
		PublicVariableEnum(const PublicVariableEnum& other);
		~PublicVariableEnum();
		PublicVariableEnum& operator=(const PublicVariableEnum& other);

		int selection = 0;
		std::vector<char*> options;
	};

	using PublicVarVariant = std::variant<int*, float*, double*, char*, std::string*, Vector2*, Vector3*, GameObject*, Component*, Color*, PublicVariableEnum*, Asset<Model>*, Asset<Script>*, Asset<Texture>*, Asset<Shader>*>;

    enum class PublicVariableType {
		Int=0, Float, Double, Char, String, Vec2, Vec3, 
		GameObject, Component, Color, PublicVariableEnum,
		Model, Script, Texture, Shader,
		None
	};

	constexpr int toInt(const PublicVariableType& e) noexcept {
		return static_cast<int>(e);
	}

	class PublicVariable {
	public:
		PublicVariable(PublicVarVariant data, std::string name);
		PublicVariable(PublicVarVariant data, std::string name, const float& sliderSpeed);

		void setData(std::string& str);
		std::string getPublicVariableString();

		static PublicVariableType getPublicVariableType(std::string& str);

		PublicVarVariant data;
		std::string name;
		float editor_sliderSpeed;
	};

};