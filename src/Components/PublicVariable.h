#pragma once
#include <string>

namespace glGame {

    enum class PublicVariableType {
		Int=0, Float, Double, Char, String, GameObject, Component, Vec2, Vec3, Color,
		Model, // Script, Texture etc
		None
	};

	class PublicVariable {
	public:
		void* data;
		PublicVariableType variableType;
		std::string name;
		float editor_sliderSpeed;

		PublicVariable(void* data, PublicVariableType variableType, std::string name);
		PublicVariable(void* data, PublicVariableType variableType, std::string name, float sliderSpeed);

		void setData(std::string& str);

		static PublicVariableType getPublicVariableType(std::string& str);
	};

};