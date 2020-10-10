#pragma once
#include <iostream>
#include "Math/Vector.h"
#include "Keycodes.h"

namespace glGame {
	class Event;
	class Window;

	class Input {
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static bool GetKeyDown(const int& keycode);
		static bool GetKey(const int& keycode);
		static bool GetKeyUp(const int& keycode);

		static bool GetMouseKeyDown(const int& keycode);
		static bool GetMouseKey(const int& keycode);
		static Vector2 GetMousePosition();
		static Vector2 GetMouseViewportPositon();
		static Vector2 GetMouseDelta();
		static Vector2 GetMouseViewportDelta();
		static float GetMouseX();
		static float GetMouseY();
		static float GetMouseDeltaX();
		static float GetMouseDeltaY();

		static void setViewportOffset(Vector2 offset);
		static void setViewportSize(Vector2 size);
		static void SetCursorMode(const int& cursorMode);

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
		static Vector2 m_viewportOffset, m_viewportSize;
		
		static Window* m_window;
		friend class Application;
	};
}