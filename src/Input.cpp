#include "Input.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include <algorithm>
#include <cstring>

namespace glGame {
	bool Input::m_keys[KEY_LAST + 1] = { false };
	bool Input::m_keysPrevious[KEY_LAST + 1] = { false };
	double Input::m_xpos = 0.0, Input::m_ypos = 0.0, Input::m_prevXpos = 0.0, Input::m_prevYpos = 0.0;
	bool Input::m_mouseKeys[MOUSE_BUTTON_LAST + 1] = { false };
	bool Input::m_mouseKeysPrevious[MOUSE_BUTTON_LAST + 1] = { false };

	Input* Input::s_instance = new Input();

	bool Input::getKeyDown(int keycode) {
		if(keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return m_keys[keycode];
	}

	bool Input::getKey(int keycode) {
		if(keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return m_keys[keycode] && !m_keysPrevious[keycode];
	}

	bool Input::getKeyUp(int keycode) {
		if (keycode < 0 || keycode > KEY_LAST) {
			return false;
		}

		return !m_keys[keycode] && m_keysPrevious[keycode];
	}

	bool Input::getMouseKeyDown(int keycode) {
		if(keycode < 0 || keycode > MOUSE_BUTTON_LAST) {
			return false;
		}
		return m_mouseKeys[keycode];
	}

	bool Input::getMouseKey(int keycode) {
		if(keycode < 0 || keycode > MOUSE_BUTTON_LAST) {
			return false;
		}
		return m_mouseKeys[keycode] && !m_mouseKeysPrevious[keycode];
	}

	std::pair<float, float> Input::getMousePosition() {
		return std::pair<float, float>(m_xpos, m_ypos);
	}

	std::pair<float, float> Input::getMouseDelta() {
		return std::pair<float, float>(m_xpos - m_prevXpos, m_ypos - m_prevYpos);
	}

	float Input::getMouseX() {
		return m_xpos;
	}

	float Input::getMouseY() {
		return m_ypos;
	}

	float Input::getMouseDeltaX() {
		return m_xpos - m_prevXpos;
	}

	float Input::getMouseDeltaY() {
		return m_ypos - m_prevYpos;
	}

	const void Input::handleEvent(Event* e) {
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

	void Input::update() {
		std::memcpy(m_keysPrevious, m_keys, KEY_LAST);
		std::memcpy(m_mouseKeysPrevious, m_mouseKeys, MOUSE_BUTTON_LAST);

		m_prevXpos = m_xpos;
		m_prevYpos = m_ypos;
	}

}