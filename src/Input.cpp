#include "Input.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseMoveEvent.h"

#include <algorithm>
#include <cstring>

namespace glGame {
	bool Input::m_keys[KEY_LAST + 1] = { false };
	bool Input::m_keysPrevious[KEY_LAST + 1] = { false };
	double Input::m_xpos = 0.0, Input::m_ypos = 0.0, Input::m_prevXpos = 0.0, Input::m_prevYpos = 0.0;

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
		return true;
	}

	bool Input::getMouseKey(int keycode) {
		return true;
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
			MouseMoved* mouseMovedEvent = (MouseMoved*)e;
			
			m_xpos = mouseMovedEvent->xpos;
			m_ypos = mouseMovedEvent->ypos;
		}
	}

	void Input::update() {
		std::memcpy(m_keysPrevious, m_keys, KEY_LAST);

		m_prevXpos = m_xpos;
		m_prevYpos = m_ypos;
	}

}