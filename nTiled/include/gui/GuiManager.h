#pragma once

#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>

#include <vector>

#include "camera\Camera.h"
#include "gui\GuiElement.h"
namespace nTiled_gui {
	class GuiManager {
	public:
		// Constructors
		GuiManager(Camera& camera);
		GuiManager(ImVec4 clear_color, 
			       Camera& camera);

		// Member Functions
		void init(GLFWwindow& window);
		void update();
		void render();

		// TODO: fix this
		ImVec4 getClearColor();
	private:
		ImVec4 clear_color;
		bool show_test_window;
		bool show_another_window;
		bool camera_has_focus;

		Camera* active_camera;
		std::vector<GuiElement*> gui_elements;
	};
}