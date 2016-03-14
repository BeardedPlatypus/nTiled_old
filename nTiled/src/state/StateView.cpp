#include "state\StateView.h"

#define VIEWPORT_X 1200
#define VIEWPORT_Y 1200

namespace nTiled {
namespace state {

View::View(camera::Camera camera,
           camera::CameraControl* p_camera_control,
           glm::uvec2 viewport) :
  camera(camera),
  camera_control(p_camera_control),
  viewport(viewport) {
}

View::~View() {
  // Delete camera control
  delete this->camera_control;
}

} // state
} // nTiled