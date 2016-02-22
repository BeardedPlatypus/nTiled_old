#include "state\State.h"

#include <fstream>
#include <string>
#include <map>

#include <rapidjson\document.h>

#include <glm\gtc\matrix_transform.hpp>

#include "world\ObjectConstructors.h"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#endif // DEBUG

using namespace nTiled_pipeline;
using namespace nTiled_world;

void nTiled_state::parseGeometry(std::string path,
	                             World& world) {
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
	std::map<std::string, ShaderId> strToId =
	{	
		{"forward_solid", ShaderId::ForwardSolid },
		{ "forward_basic_vertex", ShaderId::ForwardBasicVertex },
		{ "forward_basic_frag", ShaderId::ForwardBasicFrag },
		{ "forward_attenuated", ShaderId::ForwardAttenuated },
		{ "forward_debug_core_light", ShaderId::ForwardDebugCoreLight },
		{ "forward_debug_cutoff_light", ShaderId::ForwardDebugCutOffLight }
	};

	std::map<std::string, AssImpConstructor> obj_constructor_catalog =
		std::map<std::string, AssImpConstructor>();

	auto& objects_json = geometry["objects"];
	for (rapidjson::Value::ConstValueIterator itr = objects_json.Begin(); 
	     itr != objects_json.End(); 
		 ++itr) {
		// get object constructor
		AssImpConstructor* obj_constructor_p;

		// check if mesh constructor already exists
		std::string mesh_id = (*itr)["mesh_id"].GetString();
		std::map<std::string, AssImpConstructor>::iterator it =
			obj_constructor_catalog.find(mesh_id);
		if (it != obj_constructor_catalog.end()) {
			obj_constructor_p = &(it->second);
		}
		else {
			obj_constructor_catalog.insert(
				std::pair<std::string, AssImpConstructor>(
					mesh_id, AssImpConstructor(world, mesh_map[mesh_id])));
			obj_constructor_p = &(obj_constructor_catalog[mesh_id]);
		}

		// get attributes of the object
		std::string name = (*itr)["name"].GetString();
		ShaderId shader_id = strToId[(*itr)["shader_id"].GetString()];

		
		auto& translation_json = (*itr)["translation"];
		glm::vec3 translation = glm::vec3(translation_json["x"].GetFloat(),
			                              translation_json["y"].GetFloat(),
		                                  translation_json["z"].GetFloat());

		glm::mat4 transformation_matrix = glm::translate(glm::mat4(1.0), translation);

		obj_constructor_p->add(name,
			                   shader_id,
			                   transformation_matrix);
	}
}