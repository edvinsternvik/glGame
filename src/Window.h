#pragma once
#include <GLFW/glfw3.h>
#include "Events/Event.h"

#include <functional>

namespace glGame {

	class Window {
	public:
		Window(std::string& title, int width, int height);
		~Window();

		void swapBuffers();
		void pollEvents();
		inline void setEventFunction(std::function<void(Event&)> eventFunction) { m_data.eventFunction = eventFunction; }

		GLFWwindow* getWindow() const; 
		
	public:

	private:
		void setEventCallbackFunctions();

	private:
		GLFWwindow* m_window;

		struct windowData {
			std::string title;
			unsigned int width, height;
			std::function<void(Event&)> eventFunction;
		};

		windowData m_data;
	};
}