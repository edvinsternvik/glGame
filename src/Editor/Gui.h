#pragma once
#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <vector>
#include <memory>
#include <functional>
#include "../Events/EditorEvent.h"

class GLFWwindow;

namespace glGame {

	class Gui {

	public:
		Gui(GLFWwindow* window);

		void OnGuiRender();
		inline void setEventFunction(std::function<void(Event&)> eventFunction) { m_eventFunction = eventFunction; }
		void addWindow(std::unique_ptr<GuiWindow> window);

	private:
		void init(GLFWwindow* window);
		std::vector<std::unique_ptr<GuiWindow>> m_windows;

	private:
		ImGuiWindowFlags m_windowFlags;
		std::function<void(Event&)> m_eventFunction;
	};

}