#pragma once
#include "Event.h"

namespace glGame {

	class MouseMoved : public Event {
	public:
		MouseMoved(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}

		virtual EventType getEventType() const override { return EventType::MouseMoved; }

		virtual std::string getName() const override { return "MouseMoved"; }

		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Mouse; }

	public:
		double xpos, ypos;
	};

	class MouseButton : public Event {
	public:
		MouseButton(int button, int mod) : button(button), mod(mod) {}
	public:
		int button, mod;
	};

	class MouseButtonPressed : public MouseButton {
	public:
		MouseButtonPressed(int button, int mod) : MouseButton(button, mod) {}
		virtual EventType getEventType() const override { return EventType::MouseButtonPressed; }
		virtual std::string getName() const override { return "MouseButtonPressed"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Mouse + (int)EventCategory::MouseButton; }
	};

	class MouseButtonReleased : public MouseButton {
	public:
		MouseButtonReleased(int button, int mod) : MouseButton(button, mod) {}
		virtual EventType getEventType() const override { return EventType::MouseButtonReleased; }
		virtual std::string getName() const override { return "MouseButtonReleased"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Input + (int)EventCategory::Mouse + (int)EventCategory::MouseButton; }
	};
}