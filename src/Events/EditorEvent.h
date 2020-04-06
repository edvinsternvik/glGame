#pragma once
#include "Event.h"

namespace glGame {

	class GameStartEvent : public Event {
		virtual EventType getEventType() const override { return EventType::GameStart; }
		virtual std::string getName() const override { return "GameStart"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
	};

    class GameEndEvent : public Event {
		virtual EventType getEventType() const override { return EventType::GameEnd; }
		virtual std::string getName() const override { return "GameEnd"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
	};

    class SaveSceneEvent : public Event {
		virtual EventType getEventType() const override { return EventType::SaveScene; }
		virtual std::string getName() const override { return "SaveScene"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
	};

	class GameFocused : public Event {
		virtual EventType getEventType() const override { return EventType::GameFocused; }
		virtual std::string getName() const override { return "GameFocused"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
	};

	class GameUnfocused : public Event {
		virtual EventType getEventType() const override { return EventType::GameUnfocused; }
		virtual std::string getName() const override { return "GameUnfocused"; }
		virtual int getCategoryFlags() const override { return (int)EventCategory::Editor; }
	};
}