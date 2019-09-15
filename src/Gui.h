#pragma once
#include "GuiWindow.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <vector>
#include <memory>

class GLFWwindow;

namespace glGame {

	class Gui {

	public:
		Gui(GLFWwindow* window);

		void OnGuiRender();

	public:
		std::vector<std::unique_ptr<GuiWindow>> m_windows;

	private:
		void init(GLFWwindow* window);
	};

}