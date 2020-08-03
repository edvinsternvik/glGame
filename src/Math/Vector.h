#pragma once
#include <string>
#include <iostream>

namespace glGame {

	namespace internal {

		template<typename T>
		class Vector2 {
		public:
			Vector2(T x, T y) : x(x), y(y) {}
			Vector2() : x(0), y(0) {}

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
			T x, y;
		};

		template<typename T>
		class Vector3 {
		public:
			Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
			Vector3() : x(0), y(0), z(0) {}

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
			T x, y, z;
		};
	}

	using Vector2 = internal::Vector2<float>;
	using Vector2i = internal::Vector2<int>;
	using Vector3 = internal::Vector3<float>;
	using Vector3i = internal::Vector3<int>;

}