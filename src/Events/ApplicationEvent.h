#pragma once
#include "Event.h"

namespace glGame {

	class WindowCloseEvent : public Event {
		virtual EventType getEventType() const override { return EventType::WindowClose; }

		virtual std::string getName() const override { return "WindowClose"; }

		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; }
	};

	class WindowResizeEvent : public Event {
		virtual EventType getEventType() const override { return EventType::WindowResize; }
		
		virtual std::string getName() const override { return "WindowResize"; }
		
		virtual int getCategoryFlags() const override { return (int)EventCategory::Application; }
	};
}