#pragma once
#include <string>

#define BIT(x) (1 << x)

namespace glGame {


	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPress, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		GameStart, GameEnd, SaveScene, GameFocused, GameUnfocused
	};

	enum class EventCategory {
		None = 0,
		Application = BIT(0),
		Input = BIT(1),
		Keyboard = BIT(2),
		Mouse = BIT(3),
		MouseButton = BIT(4),
		Editor = BIT(5)
	};

	class Event {
	public:
		virtual EventType getEventType() const = 0;
		virtual std::string getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		inline bool isInCategory(EventCategory category) {
			return getCategoryFlags() & (int)category;
		}
	};
}