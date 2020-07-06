#pragma once
#include <iostream>
#include "Math/Vector.h"
#include "Keycodes.h"

namespace glGame {
	class Event;

	class Input {
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool GetKeyDown(int keycode);
		static bool GetKey(int keycode);
		static bool GetKeyUp(int keycode);

		static bool GetMouseKeyDown(int keycode);
		static bool GetMouseKey(int keycode);
		static Vector2 GetMousePosition();
		static Vector2 GetMouseDelta();
		static float GetMouseX();
		static float GetMouseY();
		static float GetMouseDeltaX();
		static float GetMouseDeltaY();

		static const void HandleEvent(Event* e);
		static void Update();

	private:
		Input() = default;
		

	private:
		static Input* s_instance;

		static bool m_keys[KEY_LAST + 1];
		static bool m_keysPrevious[KEY_LAST + 1];
		static double m_xpos, m_ypos;
		static double m_prevXpos, m_prevYpos;
		static bool m_mouseKeys[MOUSE_BUTTON_LAST + 1];
		static bool m_mouseKeysPrevious[MOUSE_BUTTON_LAST + 1];
	};
}