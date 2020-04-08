#pragma once
#include <string>
#include <iostream>

namespace glGame {

	class Vector2 {
	public:
		Vector2(float x, float y);
		Vector2();
		Vector2(std::string& str);// Str format {0.0000,0.0000}

		Vector2 operator+(const Vector2& other) {
			return Vector2(this->x + other.x, this->y + other.y);
		}

		Vector2 operator-(const Vector2& other) {
			return Vector2(this->x - other.x, this->y - other.y);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
			return os << "{ " << v.x << ", " << v.y << " }";
		}

	public:
		float x, y;
	};

	class Vector3 {
	public:
		Vector3(float x, float y, float z);
		Vector3();
		Vector3(std::string& str); // Str format {0.0000,0.0000,0.0000}

		Vector3 operator+(const Vector3& other) {
			return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		Vector3 operator-(const Vector3& other) {
			return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
			return os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
		}

	public:
		float x, y, z;
	};


}