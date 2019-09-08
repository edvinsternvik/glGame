#include "Window.h"
#include <iostream>

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseMoveEvent.h"

namespace glGame {

	Window::Window(std::string& title, int width, int height) {
		if (!glfwInit()) {
			std::cout << "Could not initialize glfw" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!m_window) {
			glfwTerminate();
			std::cout << "Could not create window" << std::endl;
			exit(EXIT_FAILURE);
		}

		m_data.title = title;
		m_data.width = width;
		m_data.height = height;

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);

		glfwSwapInterval(true);

		setEventCallbackFunctions();
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::swapBuffers() {
		glfwSwapBuffers(m_window);
	}

	void Window::pollEvents() {
		glfwPollEvents();
	}

	GLFWwindow* Window::getWindow() const {
		return m_window;
	}

	void Window::setEventCallbackFunctions() {
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.eventFunction(e);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			if(action == GLFW_PRESS) {
				KeyPress e(key, scancode, mod);
				data.eventFunction(e);
			}
			else if(action == GLFW_RELEASE) {
				KeyReleased e(key, scancode, mod);
				data.eventFunction(e);
			}
			else if(action == GLFW_REPEAT) {
				KeyTyped e(key, scancode, mod);
				data.eventFunction(e);
			}
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			windowData& data = *(windowData*)glfwGetWindowUserPointer(window);

			MouseMoved e(xpos, ypos);
			data.eventFunction(e);
		});
	}
}