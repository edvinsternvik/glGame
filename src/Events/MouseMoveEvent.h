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
}