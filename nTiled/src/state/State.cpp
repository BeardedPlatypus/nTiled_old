#include "state\State.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\LightConstructor.h"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

// TODO add graceful error handling
namespace nTiled {
namespace state {

State::State(camera::Camera camera,                  // view
             camera::CameraControl* camera_control,
             glm::uvec2 viewport,
             world::World world,                     // world
             std::map<pipeline::ForwardShaderId,     // shading
                      pipeline::ForwardShader*> forward_shader_catalog,
             std::map<pipeline::DeferredShaderId,
                      pipeline::DeferredShader*> deferred_shader_catalog,
             pipeline::PipelineType pipeline_type) :
    view(View(camera,
              camera_control,
              viewport)),
    world(world),
    shading(Shading(forward_shader_catalog,
                    deferred_shader_catalog,
                    pipeline_type)) {
}

State constructStateFromJson(const std::string& path) {
  // -------------------------------------------------------------------------
  // Parse scene.json
#ifdef DEBUG
  std::cout << "JSON - reading scene" << std::endl;
  std::cout << "   file: " << path << std::endl;
  std::cout << "   loading ... ";
#endif // DEBUG
  // stream file
  std::ifstream ifs(path);
  std::string config_file((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));
#ifdef DEBUG
  std::cout << "DONE" << std::endl;
#endif // DEBUG

#ifdef DEBUG
  std::cout << "   parsing ... ";
#endif // DEBUG
  // parse json
  rapidjson::Document config;
  config.Parse(config_file.c_str());
#ifdef DEBUG
  std::cout << "DONE" << std::endl;
#endif // DEBUG
  // Build Shading Component 
  // ------------------------------------------------------------------------
  // Pipeline type
  pipeline::PipelineType pipeline_type;
  std::string pipeline_type_str = config["pipeline"].GetString();
  if (pipeline_type_str.compare("DEFERRED") == 0) {
    pipeline_type = pipeline::PipelineType::Deferred;
  } else if (pipeline_type_str.compare("FORWARD") == 0) {
    pipeline_type = pipeline::PipelineType::Forward;
  } else if (pipeline_type_str.compare("FORWARD-DEBUG") == 0) {
    pipeline_type = pipeline::PipelineType::ForwardDebug;
  } else {
    pipeline_type = pipeline::PipelineType::Forward;
  }

#ifdef DEBUG
  std::cout << "DONE" << std::endl;
  std::cout << "      type: " << pipeline_type_str << std::endl;
#endif // DEBUG

#ifdef DEBUG
  std::cout << "   constructing Camera ... ";
#endif // DEBUG
  // Build View Component
  // ------------------------------------------------------------------------
  // camera control
  std::string camera_control_string = config["camera"]["control"].GetString();

  //TurnTableCameraControl* camera_control_p = new TurnTableCameraControl();

  camera::CameraControl* camera_control = nullptr;
  if (camera_control_string.compare("TURNTABLE") == 0) {
    camera_control = new camera::TurnTableCameraControl();
  } else {
    camera_control = new camera::TurnTableCameraControl();
  }

  // viewport
  auto& viewport_json = config["viewport"];

  glm::uvec2 viewport = glm::uvec2(viewport_json["width"].GetUint(),
                                   viewport_json["height"].GetUint());

  // camera construction
  auto& eye_json = config["camera"]["eye"];
  glm::vec3 camera_eye = glm::vec3(eye_json["x"].GetFloat(),
                                   eye_json["y"].GetFloat(),
                                   eye_json["z"].GetFloat());

  auto& center_json = config["camera"]["center"];
  glm::vec3 camera_center = glm::vec3(center_json["x"].GetFloat(),
                                      center_json["y"].GetFloat(),
                                      center_json["z"].GetFloat());


  auto& up_json = config["camera"]["up"];
  glm::vec3 camera_up = glm::vec3(up_json["x"].GetFloat(),
                                  up_json["y"].GetFloat(),
                                  up_json["z"].GetFloat());

  float fovy = config["camera"]["fovy"].GetFloat();
  float aspect = config["camera"]["aspect"].GetFloat();

  float z_near = config["camera"]["clip"]["near"].GetFloat();
  float z_far = config["camera"]["clip"]["far"].GetFloat();

  camera::Camera camera = camera::Camera(camera_control,
                                         camera_eye,
                                         camera_center,
                                         camera_up,
                                         fovy,
                                         aspect,
                                         z_near,
                                         z_far);
#ifdef DEBUG
  std::cout << "DONE" << std::endl;
  std::cout << "      control: " << camera_control_string << std::endl;
  std::cout << "      eye:    ( " << camera_eye.x << ", " <<
    camera_eye.y << ", " <<
    camera_eye.z << ") " << std::endl;

  std::cout << "      center: ( " << camera_center.x << ", " <<
    camera_center.y << ", " <<
    camera_center.z << ") " << std::endl;

  std::cout << "      up:     ( " << camera_up.x << ", " <<
    camera_up.y << ", " <<
    camera_up.z << ") " << std::endl;
  std::cout << "      fovy:   " << fovy << std::endl;
  std::cout << "      aspect: " << aspect << std::endl;
  std::cout << "      z clipping: ( " << z_near << ", " << z_far << ")" << std::endl;
#endif // DEBUG

  // Build World Component
  // ------------------------------------------------------------------------
  // construct world
  world::World world = world::World();

#ifdef DEBUG
  std::cout << "   constructing Geometry: " << std::endl;
#endif // DEBUG
  // load geometry
  auto& geometry_array_json = config["geometry"];
  for (rapidjson::Value::ConstValueIterator itr = geometry_array_json.Begin();
  itr != geometry_array_json.End();
    ++itr) {

    std::string geometry_path = (*itr)["path"].GetString();
#ifdef DEBUG
    std::cout << "      file: " << geometry_path << std::endl;
#endif // DEBUG
    parseGeometry(geometry_path, world);
  }

#ifdef DEBUG
  std::cout << "   constructing Light: " << std::endl;
#endif // DEBUG
  // Load lights
  world::PointLightConstructor light_constructor =
    world::PointLightConstructor(world);

  auto& lights_array_json = config["lights"];
  for (rapidjson::Value::ConstValueIterator itr = lights_array_json.Begin();
  itr != lights_array_json.End();
    ++itr) {
    std::string lights_path = (*itr)["path"].GetString();
    parseLights(lights_path, light_constructor);
  }

  return State(camera,
               camera_control,
               viewport,
               world,
               std::map<pipeline::ForwardShaderId,
                        pipeline::ForwardShader*>(),
               std::map<pipeline::DeferredShaderId,
                        pipeline::DeferredShader*>(),
               pipeline_type);
}

} // state
} // pipeline




