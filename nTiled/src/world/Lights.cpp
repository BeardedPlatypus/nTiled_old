#include "world\Lights.h"

namespace nTiled {
namespace world {

// ----------------------------------------------------------------------------
//  Constructors
// ----------------------------------------------------------------------------
PointLight::PointLight(glm::vec4 position,
                       glm::vec3 intensity,
                       float cutoff_radius,
                       bool is_emitting,
                       const Object& core,
                       const Object& light_volume) :
  position(position),
  intensity(intensity),
  cutoff_radius(cutoff_radius),
  is_emitting(is_emitting),
  core(core),
  light_volume(light_volume) {
}

PointLight::PointLight(glm::vec4 position,
                       glm::vec3 intensity,
                       float cutoff_radius,
                       const Object& core,
                       const Object& light_volume) :
  PointLight(position,
             intensity,
             cutoff_radius,
             true,
             core,
             light_volume) {}

} // world
} // nTiled