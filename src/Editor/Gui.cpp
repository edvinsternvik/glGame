#include "Gui.h"
#include <iostream>

glGame::Gui::Gui(GLFWwindow* window) {
	m_windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar 
			| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus 
			| ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	init(window);
}

void glGame::Gui::OnGuiRender() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	
	ImGui::NewFrame();

	//Viewport
	bool viewportOpen = true;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpaceBegin", &viewportOpen, m_windowFlags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiID dockspaceID = ImGui::GetID("Dockspace");
	ImGui::DockSpace(dockspaceID, ImVec2(0.0, 0.0));

	//Menu bar
	if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
			if(ImGui::SmallButton("save scene")) {
				SaveSceneEvent e;
				if(m_eventFunction) m_eventFunction(e);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			for(int i = 0; i < m_windows.size(); ++i) {
				const char* windowName = m_windows[i]->getWindowName();

				if(m_windows[i]->isOpen) {
					ImGui::Text(windowName);
				}
				else {
					if(ImGui::SmallButton(windowName)) {
						m_windows[i]->isOpen = true;
					}
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	//Render Windows
	for(int i = 0; i < m_windows.size(); ++i) {
		if(m_windows[i]->isOpen) {
			ImGui::Begin(m_windows[i]->getWindowName(), &m_windows[i]->isOpen, m_windows[i]->windowFlags);
			m_windows[i]->renderWindow();
			ImGui::End();
		}
	}


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

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;

	//Call imgui cleanup functions in destructor
}
