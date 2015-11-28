#pragma once

#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>

namespace nTiled_gui {
	class GuiManager {
	public:
		// Constructors
		GuiManager();
		GuiManager(ImVec4 clear_color);

		// Member Functions
		void init(GLFWwindow& window);
		void update();
		void display();

		// TODO: fix this
		ImVec4 getClearColor();
	private:
		ImVec4 clear_color;
		bool show_test_window;
		bool show_another_window;
	};
}