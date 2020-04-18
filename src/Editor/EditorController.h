#pragma once
#include "../Components/Component.h"

namespace glGame {

	class EditorController : public Component {
	public:
		EditorController();

		virtual std::string getName() const { return "EditorController"; }

		virtual void update(float deltatime) override;
	};

}
