#pragma once

#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

#include <vector>

#include "state\State.h"
#include "camera\Camera.h"
#include "gui\GuiElement.h"
namespace nTiled_gui {
	class GuiManager {
	public:
		// Constructors
		GuiManager(nTiled_state::State& state);
		GuiManager(ImVec4 clear_color, 
			       nTiled_state::State& state);

		// Member Functions
		void init(GLFWwindow& window);
		void update();
		void render();

		// TODO: fix this
		ImVec4 getClearColor();
	private:
		ImVec4 clear_color;

		// element
		bool show_test_window;
		bool show_another_window;
		bool camera_has_focus;

		nTiled_state::State& state;
		std::vector<GuiElement*> gui_elements;
	};
}