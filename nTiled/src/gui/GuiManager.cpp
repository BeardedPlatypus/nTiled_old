#include "gui\GuiManager.h"

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
nTiled_gui::GuiManager::GuiManager(ImVec4 clear_color) :
	clear_color(clear_color),
	show_test_window(true),
	show_another_window(false) {}

nTiled_gui::GuiManager::GuiManager() : 
	nTiled_gui::GuiManager(ImColor(114, 144, 154)) {}

// ----------------------------------------------------------------------------
//  Member functions
// ----------------------------------------------------------------------------
void nTiled_gui::GuiManager::init(GLFWwindow& window) {
	ImGui_ImplGlfwGL3_Init(&window, true);
}

void nTiled_gui::GuiManager::update() {
	// Poll events and write to ImGUI.
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();

	// Handle updates to Camera/view
	
	// Draw gui elements.
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void nTiled_gui::GuiManager::display() {
	ImGui::Render();
}

ImVec4 nTiled_gui::GuiManager::getClearColor() {
	return this->clear_color;
}