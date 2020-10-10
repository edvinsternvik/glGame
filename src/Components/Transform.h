#pragma once
#include "Component.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace glGame {

	class Transform : public Component {
	public:
		Transform();

		virtual std::string getName() const { return "Transform"; }

		void move(const Vector3& vector);
		void move(const float& x, const float& y, const float& z);
		void rotate(const Vector3& vector);
		void rotate(const float& x, const float& y, const float& z);
		void rotate(const Quaternion& quaternion);
		void resize(const Vector3& s);

		glm::mat4 getTransformMatrix() const;
		Vector3 getEulerAngles() const;

	public:
		Vector3 position, scale;
		Quaternion orientation;
	};

}
