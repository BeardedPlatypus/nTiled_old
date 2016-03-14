#include "world\LightConstructor.h"

#define CORE_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/scenes/debug/light/light_core.obj")
#define RADIUS_PATH std::string("C:/Users/Monthy/Documents/projects/thesis/scenes/debug/light/light_radius_b.obj")

namespace nTiled {
namespace world {

PointLightConstructor::PointLightConstructor(World& world) :
  world(world),
  core_constructor(AssImpConstructor(CORE_PATH,
                                     world)),
  cutoff_constructor(AssImpConstructor(RADIUS_PATH,
                                       world)) {
}

void PointLightConstructor::add(const std::string& name,
                                glm::vec3 intensity,
                                float cutoff_radius,
                                bool is_emitting,
                                glm::mat4 transformationMatrix) {
  glm::mat4 cutoff_transform = transformationMatrix *
    glm::mat4(cutoff_radius, 0.0, 0.0, 0.0,
              0.0, cutoff_radius, 0.0, 0.0,
              0.0, 0.0, cutoff_radius, 0.0,
              0.0, 0.0, 0.0, 1.0);

  /*
  Object* core_p = this->core_constructor.add(
    name + "_core",
    nTiled_pipeline::ShaderId::ForwardDebugCoreLight,
    transformationMatrix);
    */
  Object* cutoff_p = this->cutoff_constructor.add(
    name + "_cutoff",
    pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardDebugCutOffLight),
    transformationMatrix);

  glm::vec4 position = transformationMatrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
  this->world.addLight(PointLight(position,
                                  intensity,
                                  cutoff_radius,
                                  is_emitting,
                                  *cutoff_p, //*core_p,
                                  *cutoff_p));
}

} // world
} // nTiled