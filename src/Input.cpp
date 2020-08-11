#include "Input.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Window.h"

#include <algorithm>
#include <cstring>

namespace glGame {
	bool Input::m_keys[KEY_LAST + 1] = { false };
	bool Input::m_keysPrevious[KEY_LAST + 1] = { false };
	double Input::m_xpos = 0.0, Input::m_ypos = 0.0, Input::m_prevXpos = 0.0, Input::m_prevYpos = 0.0;
	bool Input::m_mouseKeys[MOUSE_BUTTON_LAST + 1] = { false };
	bool Input::m_mouseKeysPrevious[MOUSE_BUTTON_LAST + 1] = { false };
	Vector2 Input::m_viewportOffset = Vector2(0.0, 0.0);
	Window* Input::m_window = nullptr;

	Input* Input::s_instance = new Input();

	bool Input::GetKeyDown(const int& keycode) {
		if(keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return m_keys[keycode] && !m_keysPrevious[keycode];
	}

	bool Input::GetKey(const int& keycode) {
		if(keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return m_keys[keycode];
	}

	bool Input::GetKeyUp(const int& keycode) {
		if (keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return !m_keys[keycode] && m_keysPrevious[keycode];
	}

	bool Input::GetMouseKeyDown(const int& keycode) {
		if(keycode < 0 || keycode > MOUSE_BUTTON_LAST) {
			return false;
		}
		return m_mouseKeys[keycode] && !m_mouseKeysPrevious[keycode];
	}

	bool Input::GetMouseKey(const int& keycode) {
		if(keycode < 0 || keycode > MOUSE_BUTTON_LAST) {
			return false;
		}
		return m_mouseKeys[keycode];
	}

	Vector2 Input::GetMousePosition() {
		return Vector2(m_xpos, m_ypos) - m_viewportOffset;
	}

	Vector2 Input::GetMouseDelta() {
		return Vector2(m_xpos - m_prevXpos, m_ypos - m_prevYpos);
	}

	float Input::GetMouseX() {
		return m_xpos + m_viewportOffset.x;
	}

	float Input::GetMouseY() {
		return m_ypos + m_viewportOffset.y;
	}

	float Input::GetMouseDeltaX() {
		return m_xpos - m_prevXpos;
	}

	float Input::GetMouseDeltaY() {
		return m_ypos - m_prevYpos;
	}

	void Input::setViewportOffset(Vector2 offset) {
		m_viewportOffset = offset;
	}

	void Input::SetCursorMode(const int& cursorMode) {
		glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, cursorMode);
	}

	const void Input::HandleEvent(Event* e) {
		if(e->isInCategory(EventCategory::Keyboard)) {
			KeyboardEvent* keyEvent = (KeyboardEvent*)e;
			if(keyEvent->keycode < 0 || keyEvent->keycode > KEY_LAST) {
				return;
			}

			if(keyEvent->getEventType() == EventType::KeyPress) {
				m_keys[keyEvent->keycode] = 1;
			}
			else if(keyEvent->getEventType() == EventType::KeyReleased) {
				m_keys[keyEvent->keycode] = 0;
			}
			//todo handle key typed
		}
		else if(e->isInCategory(EventCategory::Mouse)) {
			if(e->getEventType() == EventType::MouseMoved) {
				MouseMoved* mouseMovedEvent = (MouseMoved*)e;
				
				m_xpos = mouseMovedEvent->xpos;
				m_ypos = mouseMovedEvent->ypos;
			}
			else if(e->isInCategory(EventCategory::MouseButton)) {
				MouseButton* mouseEvent = (MouseButton*)e;
				if(mouseEvent->getEventType() == EventType::MouseButtonPressed) {
					m_mouseKeys[mouseEvent->button] = 1;
				}
				else if(mouseEvent->getEventType() == EventType::MouseButtonReleased) {
					m_mouseKeys[mouseEvent->button] = 0;
				}
			}
		}
	}

	void Input::Update() {
		std::memcpy(m_keysPrevious, m_keys, KEY_LAST);
		std::memcpy(m_mouseKeysPrevious, m_mouseKeys, MOUSE_BUTTON_LAST);

		m_prevXpos = m_xpos;
		m_prevYpos = m_ypos;
	}

}