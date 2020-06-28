#pragma once
#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <vector>
#include <memory>
#include <functional>
#include "../Events/EditorEvent.h"
#include <iostream>

class GLFWwindow;

namespace glGame {

	class Gui {

	public:
		Gui(GLFWwindow* window);

		void OnGuiRender();
		inline void setEventFunction(std::function<void(Event&)> eventFunction) { m_eventFunction = eventFunction; }
		template<class T>
		T* addWindow(std::unique_ptr<T> window, Editor* editor) {
			if(!m_eventFunction) {
				std::cout << "Event function not set : Could not add window" << std::endl;
				return nullptr;
			}
			window->setEventFunction(m_eventFunction);
			window->setEditor(editor);
			T* ret = window.get();
			m_windows.push_back(std::move(window));
			return ret;
		}

	private:
		void init(GLFWwindow* window);
		std::vector<std::unique_ptr<GuiWindow>> m_windows;

	private:
		ImGuiWindowFlags m_windowFlags;
		std::function<void(Event&)> m_eventFunction;
	};

}