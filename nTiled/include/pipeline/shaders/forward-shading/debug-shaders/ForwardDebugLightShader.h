#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shaders\forward-shading\ForwardShader.h"

namespace nTiled {
namespace pipeline {

class ForwardDebugLight : public ForwardFragLightingShader {
 public:
  ForwardDebugLight(ForwardShaderId shader_id,
                    const std::string& vertex_shader_path,
                    const std::string& fragment_shader_path);
  void loadShaders();
};

// ------------------------------------------------------------------------
class ForwardDebugCoreLight : public ForwardDebugLight {
 public:
  ForwardDebugCoreLight(ForwardShaderId shader_id,
                        const std::string& vertex_shader_path,
                        const std::string& fragment_shader_path);
  void init(const camera::Camera& camera);
  void render(const camera::Camera& camera);
};

// ------------------------------------------------------------------------
class ForwardDebugCutOffLight : public ForwardDebugLight {
 public:
  ForwardDebugCutOffLight(ForwardShaderId shader_id,
                          const std::string& vertex_shader_path,
                          const std::string& fragment_shader_path);
  void init(const camera::Camera& camera);
  void render(const camera::Camera& camera);
};

} // pipeline
} // nTiled

