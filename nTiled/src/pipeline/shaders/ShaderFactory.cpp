#include "pipeline\shaders\ShaderFactory.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shaders\forward-shading\ForwardShader.h"
#include "pipeline\shaders\forward-shading\debug-shaders\ForwardDebugLightShader.h"
#include "pipeline\shaders\deferred-shading\DeferredShader.h"
#include <iostream>

#define TILESIZE 1200 

namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  Constructor || Destructor
// ----------------------------------------------------------------------------
ShaderFactory::ShaderFactory(state::State& state) : state(state) {}

// ----------------------------------------------------------------------------
//  getForwardShader
// ----------------------------------------------------------------------------
ForwardShader* ShaderFactory::getForwardShader(ForwardShaderId shader_id) {
  std::map<ForwardShaderId, ForwardShader*>::iterator it =
    this->state.shading.forward_shader_catalog.find(shader_id);

  // shader is already constructed and thus returned
  if (it != this->state.shading.forward_shader_catalog.end()) {
    return it->second;
  } // shader is not yet constructed, and thus added
  else {
    ForwardShader* p_shader = addForwardShader(shader_id);
    return p_shader;
  }
}


// ----------------------------------------------------------------------------
//  addForwardShader
// ----------------------------------------------------------------------------
ForwardShader* ShaderFactory::addForwardShader(ForwardShaderId shader_id) {
  ForwardShader* p;

  // Regular Forward Shaders
  // ------------------------------------------------------------------------
  if (shader_id == ForwardShaderId::ForwardSolid) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/solid/solid.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/solid/solid.frag";

    // Allocate new shader batch on the stack
    p = new SolidShader(ForwardShaderId::ForwardSolid,
                        path_vert_shader,
                        path_frag_shader);
  } else if (shader_id == ForwardShaderId::ForwardBasicVertex) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLights.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLights.frag";

    // Allocate new shader batch on the stack
    p = new ForwardVertLightingShader(
      ForwardShaderId::ForwardBasicVertex,
      path_vert_shader,
      path_frag_shader);
  } else if (shader_id == ForwardShaderId::ForwardBasicFrag) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLightsFrag.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/basic/basicForwardMultipleLightsFrag.frag";

    // Allocate new shader batch on the stack
    p = new ForwardFragLightingShader(
      ForwardShaderId::ForwardBasicFrag,
      path_vert_shader,
      path_frag_shader);
  } else if (shader_id == ForwardShaderId::ForwardAttenuated) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/forward-attenuated/basicForwardMultipleLightsFrag.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/forward-attenuated/basicForwardMultipleLightsFrag.frag";

    // Allocate new shader batch on the stack
    p = new ForwardFragLightingShader(
      ForwardShaderId::ForwardAttenuated,
      path_vert_shader,
      path_frag_shader);
  }
  // Tiled Forward Shaders
  // ------------------------------------------------------------------------
  else if (shader_id == ForwardShaderId::ForwardTiled) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/forward-tiled/forward-tiled.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/forward-tiled/forward-tiled.frag";

    // Allocate new shader batch on the stack
    p = new ForwardTiledShader(
      ForwardShaderId::ForwardTiled,
      path_vert_shader,
      path_frag_shader,
      glm::uvec2(TILESIZE, TILESIZE),  // FIXME remove hard codedness of this
      state.world, state.view);
  }

  // Clustered Forward Shaders
  // ------------------------------------------------------------------------

  //  Debug Forward Shaders
  // ------------------------------------------------------------------------
  else if (shader_id == ForwardShaderId::ForwardDebugCoreLight) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/debug-shaders/core/core.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/debug-shaders/core/core.frag";

    // Allocate new shader batch on the stack
    p = new ForwardDebugCoreLight(
      ForwardShaderId::ForwardDebugCoreLight,
      path_vert_shader,
      path_frag_shader);
  } else if (shader_id == ForwardShaderId::ForwardDebugCutOffLight) {
    // Shader locations
    std::string path_vert_shader =
      "./src/pipeline/shaders-glsl/forward-shading/debug-shaders/cutoff/cutoff.vert";
    std::string path_frag_shader =
      "./src/pipeline/shaders-glsl/forward-shading/debug-shaders/cutoff/cutoff.frag";

    // Allocate new shader batch on the stack
    p = new ForwardDebugCutOffLight(
      ForwardShaderId::ForwardDebugCutOffLight,
      path_vert_shader,
      path_frag_shader);
  }

  // Add shader to the catalog and return
  this->state.shading.forward_shader_catalog.insert(
    std::pair<ForwardShaderId, ForwardShader*>(shader_id, p));
  return p;
}

// ----------------------------------------------------------------------------
//  getDeferredShader
// ----------------------------------------------------------------------------
DeferredShader* ShaderFactory::getDeferredShader(DeferredShaderId shader_id) {
  std::map<DeferredShaderId, DeferredShader*>::iterator it =
    this->state.shading.deferred_shader_catalog.find(shader_id);

  // shader is already constructed and thus returned
  if (it != this->state.shading.deferred_shader_catalog.end()) {
    return it->second;
  } // shader is not yet constructed, and thus added
  else {
    DeferredShader* p_shader = addDeferredShader(shader_id);
    return p_shader;
  }
}


DeferredShader* ShaderFactory::addDeferredShader(DeferredShaderId shader_id) {
  DeferredShader* p;

  // Deferred Shaders
  // ------------------------------------------------------------------------
  if (shader_id == DeferredShaderId::DeferredBasic) {
    // Shader locations
    std::string geometry_path_vert_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/basic/DSGeometryPass.vert";
    std::string geometry_path_frag_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/basic/DSGeometryPass.frag";
    std::string light_path_vert_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/basic/DSLightPass.vert";
    std::string light_path_frag_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/basic/DSLightPass.frag";

    // Allocate new shader batch on the stack
    p = new DeferredShader(
      DeferredShaderId::DeferredBasic,
      geometry_path_vert_shader,
      geometry_path_frag_shader,
      light_path_vert_shader,
      light_path_frag_shader,
      state.world, state.view);
  } else if (shader_id == DeferredShaderId::DeferredAttenuated) {
    // Shader locations
    std::string geometry_path_vert_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-attenuated/DSGeometryPass.vert";
    std::string geometry_path_frag_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-attenuated/DSGeometryPass.frag";
    std::string light_path_vert_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-attenuated/DSLightPass.vert";
    std::string light_path_frag_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-attenuated/DSLightPass.frag";

    // Allocate new shader batch on the stack
    p = new DeferredShader(
      DeferredShaderId::DeferredAttenuated,
      geometry_path_vert_shader,
      geometry_path_frag_shader,
      light_path_vert_shader,
      light_path_frag_shader,
      state.world, state.view);
  } else if (shader_id == DeferredShaderId::DeferredTiled) {
    // Shader locations
    std::string geometry_path_vert_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-tiled/DSGeometryPass.vert";
    std::string geometry_path_frag_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-tiled/DSGeometryPass.frag";
    std::string light_path_vert_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-tiled/DSLightPass.vert";
    std::string light_path_frag_shader =
      "./src/pipeline/shaders-glsl/deferred-shading/deferred-tiled/DSLightPass.frag";

    // Allocate new shader batch on the stack
    p = new TiledDeferredLightShader(
      DeferredShaderId::DeferredTiled,
      geometry_path_vert_shader,
      geometry_path_frag_shader,
      light_path_vert_shader,
      light_path_frag_shader,
      glm::uvec2(TILESIZE, TILESIZE),  // FIXME remove hard codedness of this
      state.world, state.view);
  }

  // Add shader to the catalog and return
  this->state.shading.deferred_shader_catalog.insert(
    std::pair<DeferredShaderId, DeferredShader*>(shader_id, p));
  return p;
}

} // pipeline
} // nTiled