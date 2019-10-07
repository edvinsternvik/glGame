#include "Gui.h"

glGame::Gui::Gui(GLFWwindow* window) {
	init(window);
}

void glGame::Gui::OnGuiRender() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	
	ImGui::NewFrame();

	for(int i = 0; i < m_windows.size(); ++i) {
		ImGui::Begin(m_windows[i]->getWindowName().c_str());
		m_windows[i]->renderWindow();
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void glGame::Gui::init(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(NULL); //NULL can be replaced by: #version 130

	//Call imgui cleanup functions in destructor
}
