#pragma once
#include "Event.h"

namespace glGame {

	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(int keycode, int scancode, int mod) : keycode(keycode), scancode(scancode), mod(mod) {}

	public:
		int keycode, scancode, mod;
	};

	class KeyPress : public KeyboardEvent {
	public:
		KeyPress(int keycode, int scancode, int mod) : KeyboardEvent(keycode, scancode, mod) {}

		virtual EventType getEventType() const override { return EventType::KeyPress; }

		virtual std::string getName() const override { return "KeyPress"; }

		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Keyboard; }
	};

	class KeyReleased : public KeyboardEvent {
	public:
		KeyReleased(int keycode, int scancode, int mod) : KeyboardEvent(keycode, scancode, mod) {}

		virtual EventType getEventType() const override { return EventType::KeyReleased; }

		virtual std::string getName() const override { return "KeyReleased"; }

		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Keyboard; }
	};

	class KeyTyped : public KeyboardEvent {
	public:
		KeyTyped(int keycode, int scancode, int mod) : KeyboardEvent(keycode, scancode, mod) {}

		virtual EventType getEventType() const override { return EventType::KeyTyped; }

		virtual std::string getName() const override { return "KeyTyped"; }

		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Keyboard; }
	};
}