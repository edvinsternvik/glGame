#pragma once
#include <iostream>
#include "Keycodes.h"

namespace glGame {
	class Event;

	class Input {
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool getKeyDown(int keycode);
		static bool getKey(int keycode);

		static bool getMouseKeyDown(int keycode);
		static bool getMouseKey(int keycode);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();

		static const void handleEvent(Event* e);
		static void update();

	private:
		Input() = default;
		

	private:
		static Input* s_instance;

		static bool m_keys[KEY_LAST + 1];
		static bool m_keysPrevious[KEY_LAST + 1];
		static double m_xpos, m_ypos;
	};
}