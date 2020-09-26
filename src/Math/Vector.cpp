#include "Vector.h"

std::ostream& operator<<(std::ostream& out, const glm::vec2& v) {
	return out << "{ " << v.x << ", " << v.y << " }";
}
std::ostream& operator<<(std::ostream& out, const glm::ivec2& v) {
	return out << "{ " << v.x << ", " << v.y << " }";
}
std::ostream& operator<<(std::ostream& out, const glm::vec3& v) {
	return out << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
}
std::ostream& operator<<(std::ostream& out, const glm::ivec3& v) {
	return out << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
}
std::ostream& operator<<(std::ostream& out, const glm::vec4& v) {
	return out << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
}
std::ostream& operator<<(std::ostream& out, const glm::ivec4& v) {
	return out << "{ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " }";
}