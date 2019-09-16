#pragma once
#include <string>

namespace glGame{

	class GameObject {
	public:
		GameObject(std::string& name);

		void onUpdate();

	public:
		std::string name;

	};

}