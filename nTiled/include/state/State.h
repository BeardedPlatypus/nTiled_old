#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>

#include "state\StateShading.h"
#include "state\StateView.h"

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"
#include "world\LightConstructor.h"


namespace nTiled {
namespace state {

/*!
 State holds all attributes of this nTiled renderer
 */
struct State {
 public:
  // Constructor
  State(camera::Camera camera,                  // view
        camera::CameraControl* camera_control,
        glm::uvec2 viewport,
        world::World world,                     // world
        std::map<pipeline::ForwardShaderId,     // shading
                 pipeline::ForwardShader*> forward_shader_catalog,
        std::map<pipeline::DeferredShaderId,
                 pipeline::DeferredShader*> deferred_shader_catalog,
        pipeline::PipelineType pipeline_type);

  // Member attributes
  View view;
  world::World world;
  Shading shading;
};

/*!
 Construct a State from the specified scene.json file

 Args:
     path (const std::string&): Path to the scene.json file
 Returns:
     A state corresponding with the provided scene.json file
 */
State constructStateFromJson(const std::string& path);

// Parsing functions
/*! 
 Parse the lights from a specified lights.json file

 Parse the lights from a specified lights.json file,
 these will be added to the world with the provided 
 LightConstructor object

 Args:
     path (const std::string& ): Path to the lights.json file
     constructor (nTiled::world::LightConstructor&):
       Light constructor used to add the specified lights to 
       the world the LightConstructor belongs to
 */
void parseLights(const std::string& path,
                 world::LightConstructor& constructor);

/*!
 Parse the geometry form a specified geometry.json file

 Parse the geometry from a specified geometry.json file, 
 these objects will be added to the referenced world.

 Args:
     path (const std::string&): Path to the geometry.json file
     world (nTiled::world::World&): world to which this geometry 
                                    should be added
 */
void parseGeometry(const std::string& path,
                   world::World& world);
} // state
} // nTiled

