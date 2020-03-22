#pragma once
#include <string>

namespace glGame {

	class Vector2 {
	public:
		Vector2(float x, float y);
		Vector2(std::string& str);// Str format {0.0000,0.0000}

	public:
		float x, y, z;
	};

	class Vector3 {
	public:
		Vector3(float x, float y, float z);
		Vector3(std::string& str); // Str format {0.0000,0.0000,0.0000}

	public:
		float x, y, z;
	};


}