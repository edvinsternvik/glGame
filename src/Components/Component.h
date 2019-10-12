#pragma once
#include <string>

namespace glGame {

	class Component {
	public:
		Component() {}

		virtual std::string getName() const = 0;
		virtual void update() = 0;

	};

}