#include "state\State.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
// File handling
#include <fstream>
#include <string>
#include <map>

// Json include
#include <rapidjson\document.h>

#include <glm\gtc\matrix_transform.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\ObjectConstructors.h"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#endif // DEBUG

namespace nTiled {
void state::parseGeometry(const std::string& path,
                          world::World& world) {
  // read file to string
#ifdef DEBUG
  std::cout << "         loading ... ";
#endif // DEBUG
  std::ifstream ifs(path);
  std::string geometry_json((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
#ifdef DEBUG
  std::cout << "DONE" << std::endl;
#endif // DEBUG

#ifdef DEBUG
  std::cout << "         parsing ... ";
#endif // DEBUG
  // parse json
  rapidjson::Document geometry;
  geometry.Parse(geometry_json.c_str());
#ifdef DEBUG
  std::cout << "DONE" << std::endl;
#endif // DEBUG


  // parse meshes
#ifdef DEBUG
  std::cout << "         parsing Meshes... ";
  unsigned int i = 0;
#endif // DEBUG
  std::map<std::string, std::string> mesh_map = std::map<std::string, std::string>();

  auto& meshes_json = geometry["meshes"];

  for (rapidjson::Value::ConstValueIterator itr = meshes_json.Begin();
  itr != meshes_json.End();
    ++itr) {
    std::string mesh_id = (*itr)["id"].GetString();

    std::string mesh_path = (*itr)["path"].GetString();

    mesh_map.insert(std::pair<std::string, std::string>(mesh_id, mesh_path));
#ifdef DEBUG
    i++;
#endif // DEBUG
  }

#ifdef DEBUG
  std::cout << i << " meshes" << std::endl;
#endif // DEBUG


  // parse objects
  std::map<std::string, pipeline::ShaderKey> strToId =
  {
    { "forward_solid", 
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardSolid) },
    { "forward_basic_vertex", 
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardBasicVertex) },
    { "forward_basic_frag", 
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardBasicFrag) },
    { "forward_attenuated", 
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardAttenuated) },
    { "forward_debug_core_light",
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardDebugCoreLight) },
    { "forward_debug_cutoff_light", 
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardDebugCutOffLight) },
    { "forward_tiled", 
      pipeline::ShaderKey(pipeline::ForwardShaderId::ForwardTiled) },
    { "deferred_basic", 
      pipeline::ShaderKey(pipeline::DeferredShaderId::DeferredBasic) },
    { "deferred_attenuated", 
      pipeline::ShaderKey(pipeline::DeferredShaderId::DeferredAttenuated) },
    { "deferred_tiled", 
      pipeline::ShaderKey(pipeline::DeferredShaderId::DeferredTiled) },
  };

  std::map<std::string, world::AssImpConstructor> obj_constructor_catalog =
    std::map<std::string, world::AssImpConstructor>();

  auto& objects_json = geometry["objects"];
  for (rapidjson::Value::ConstValueIterator itr = objects_json.Begin();
  itr != objects_json.End();
    ++itr) {
    // get object constructor
    world::AssImpConstructor* obj_constructor_p;

    // check if mesh constructor already exists
    std::string mesh_id = (*itr)["mesh_id"].GetString();
    std::map<std::string, world::AssImpConstructor>::iterator it =
      obj_constructor_catalog.find(mesh_id);
    if (it != obj_constructor_catalog.end()) {
      obj_constructor_p = &(it->second);
    } else {
      obj_constructor_catalog.insert(
        std::pair<std::string, world::AssImpConstructor>(
          mesh_id, world::AssImpConstructor(mesh_map[mesh_id],
                                            world)));
      obj_constructor_p = &(obj_constructor_catalog[mesh_id]);
    }

    // get attributes of the object
    std::string name = (*itr)["name"].GetString();
    pipeline::ShaderKey shader_key = strToId[(*itr)["shader_id"].GetString()];


    auto& translation_json = (*itr)["translation"];
    glm::vec3 translation = glm::vec3(translation_json["x"].GetFloat(),
                                      translation_json["y"].GetFloat(),
                                      translation_json["z"].GetFloat());

    glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0), translation);

    obj_constructor_p->add(name,
                           shader_key,
                           transformation_matrix);
  }
}
} // nTiled