#include "Gui.h"

glGame::Gui::Gui(GLFWwindow* window) {
	init(window);
}

void glGame::Gui::OnGuiRender() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();


	ImGui::NewFrame();

	ImGui::ShowMetricsWindow();

	for(int i = 0; i < m_windows.size(); ++i) {
		ImGui::Begin(m_windows[i]->getWindowName().c_str());
		m_windows[i]->renderWindow();
		ImGui::End();
	}
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::Button("Button");                            // Buttons return true when clicked (most widgets return true when edited/activated)

	//ImGui::Image()
		
	ImGui::SameLine();

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImGui::Begin("Hello, world2!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("Hello");
	ImGui::End();
	
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
