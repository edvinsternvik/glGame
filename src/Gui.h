#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

class GLFWwindow;

namespace glGame {

	class Gui {

	public:
		Gui(GLFWwindow* window);

		void OnGuiRender();

	private:
		void init(GLFWwindow* window);
	};

}