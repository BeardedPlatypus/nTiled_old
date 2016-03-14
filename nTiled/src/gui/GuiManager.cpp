#include "gui\GuiManager.h"

#include <glm\gtx\string_cast.hpp>
#include <GLFW/glfw3.h>

namespace nTiled {
namespace gui {

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
GuiManager::GuiManager(ImVec4 clear_color,
                       state::State& state) :
  clear_color(clear_color),
  state(state),
  show_test_window(false),
  show_another_window(false),
  camera_has_focus(false),
  gui_elements(std::vector<GuiElement*>()) {
}

GuiManager::GuiManager(state::State& state) :
  GuiManager(ImColor(114, 144, 154), state) {}

// ----------------------------------------------------------------------------
//  Member functions
// ----------------------------------------------------------------------------
void GuiManager::init(GLFWwindow& window) {
  ImGui_ImplGlfwGL3_Init(&window, true);

  this->gui_elements.push_back(&(CameraElement(this->state.view.camera)));
}

void GuiManager::update() {
  // Poll events and write to ImGUI.
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  // Handle updates to Camera/view
  if ((!io.WantCaptureMouse || !io.WantCaptureKeyboard) && io.MouseDown[0]) {
    // we assume that we're interested in seeing if we need to update the
    // camera whenever none of the GUI elements is active.
    if (camera_has_focus) {
      this->state.view.camera.update(io);
    } else {
      this->state.view.camera.toFocus(io);
      this->camera_has_focus = true;
    }
  } else if (this->camera_has_focus) {
    this->camera_has_focus = false;
  }

  // Draw gui elements.
  {
    static float f = 0.0f;
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("MouseCaptureState: %s", io.WantCaptureMouse ? "true" : "false");
    ImGui::Text("KeyboardCaptureState: %s", io.WantCaptureKeyboard ? "true" : "false");
    ImGui::Text("MouseDown: %s", io.MouseDown[0] ? "true" : "false");
    ImGui::Text("Mouse x: %f", io.MousePos.x);
    ImGui::Text("Mouse y: %f", io.MousePos.y);
    ImGui::Text("Prev Mouse x: %f", io.MousePosPrev.x);
    ImGui::Text("Prev Mouse y: %f", io.MousePosPrev.y);

    /*
    for (GuiElement* element : this->gui_elements) {
      element->render();
    }
    */
  }

  // 2. Show another simple window, this time using an explicit Begin/End pair
  if (show_another_window) {
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Another Window", &show_another_window);
    ImGui::Text("Hello");
    ImGui::End();
  }

  // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
  if (show_test_window) {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
    ImGui::ShowTestWindow(&show_test_window);
  }
}

void GuiManager::render() {
  ImGui::Render();
}

ImVec4 GuiManager::getClearColor() {
  return this->clear_color;
}

} // gui
} // nTiled