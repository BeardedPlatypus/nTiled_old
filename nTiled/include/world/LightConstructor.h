#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "world\ObjectConstructors.h"

namespace nTiled {
namespace world {

// ------------------------------------------------------------------------
//  Abstract class
class LightConstructor {
 public:
  virtual ~LightConstructor() {}
  virtual void add(const std::string& name,
                   glm::vec3 intensity,
                   float cutoff_radius,
                   bool is_emitting,
                   glm::mat4 transformationMatrix) = 0;
};

// ------------------------------------------------------------------------
//  Point lights
class PointLightConstructor : public LightConstructor {
 public:
  PointLightConstructor(World& world);
  void add(const std::string& name,
           glm::vec3 intensity,
           float cutoff_radius,
           bool is_emitting,
           glm::mat4 transformationMatrix);

 private:
  World& world;

  // Object constructors
  AssImpConstructor core_constructor;
  AssImpConstructor cutoff_constructor;
};

}
}