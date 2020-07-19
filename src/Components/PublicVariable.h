#pragma once
#include "../Math/Vector.h"
#include "../Resources/Asset.h"
#include <string>
#include <variant>
#include <memory>

namespace glGame {
	
	class GameObject;
	class Component;
	class Color;

	using PublicVarVariant = std::variant<int*, float*, double*, char*, std::string*, Vector2*, Vector3*, GameObject*, Component*, Color*, Asset<Model>*, Asset<Script>*, Asset<Texture>*, Asset<Shader>*>;

    enum class PublicVariableType {
		Int=0, Float, Double, Char, String, Vec2, Vec3, 
		GameObject, Component, Color,
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

		static PublicVariableType getPublicVariableType(std::string& str);

		PublicVarVariant data;
		std::string name;
		float editor_sliderSpeed;
	};

};