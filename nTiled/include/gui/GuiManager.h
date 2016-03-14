#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <imgui.h>
#include "gui/imgui_impl_glfw_gl3.h"

#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\State.h"
#include "camera\Camera.h"
#include "gui\GuiElement.h"


namespace nTiled {
namespace gui {

class GuiManager {
 public:
  // Constructors
  GuiManager(state::State& state);
  GuiManager(ImVec4 clear_color,
             state::State& state);

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

  state::State& state;
  std::vector<GuiElement*> gui_elements;
};

} // gui
} // nTiled