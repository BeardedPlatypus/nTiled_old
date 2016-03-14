#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "camera\Camera.h"

namespace nTiled {
namespace gui {

class GuiElement {
public:
  virtual ~GuiElement() {}
  virtual void render() = 0;
};

class CameraElement : public GuiElement {
public:
  CameraElement(camera::Camera& camera);
  void render();
private:
  camera::Camera& camera;
};

} // gui
} // nTiled
