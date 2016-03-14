#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\Object.h"

namespace nTiled {
namespace world {

struct PointLight {
 public:
  // Constructor
  PointLight(glm::vec4 position,
             glm::vec3 intensity,
             float radius,
             const Object& core,
             const Object& light_volume);
  PointLight(glm::vec4 position,
             glm::vec3 intensity,
             float radius,
             bool isEmitting,
             const Object& core,
             const Object& light_volume);

  // Properties
  glm::vec4 position;
  float cutoff_radius;

  glm::vec3 intensity;
  bool is_emitting;

  const Object& core;
  const Object& light_volume;
};

} // world
} // nTiled
