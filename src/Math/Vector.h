#pragma once
#include <string>
#include <iostream>
#include <glm/glm.hpp>

std::ostream& operator<<(std::ostream& out, const glm::vec2& v);
std::ostream& operator<<(std::ostream& out, const glm::ivec2& v);
std::ostream& operator<<(std::ostream& out, const glm::vec3& v);
std::ostream& operator<<(std::ostream& out, const glm::ivec3& v);
std::ostream& operator<<(std::ostream& out, const glm::vec4& v);
std::ostream& operator<<(std::ostream& out, const glm::ivec4& v);

namespace glGame {

	using Vector2 = glm::vec2;
	using Vector2i = glm::ivec2;
	using Vector3 = glm::vec3;
	using Vector3i = glm::ivec3;
	using Vector4 = glm::vec4;
	using Vector4i = glm::ivec4;

}