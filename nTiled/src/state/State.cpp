#include "state\State.h"

// File handling
#include <fstream>
#include <string>
#include <vector>

// Json include
#include <rapidjson\document.h>

#include "world\LightConstructor.h"

#define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG


// TODO add graceful error handling
using namespace nTiled_state;
State::State(std::string path): 
	shader_catalog(std::map<nTiled_pipeline::ShaderId, 
		                    nTiled_pipeline::ShaderBatch*>()) {
	// Read file to string
#ifdef DEBUG
	std::cout << "JSON - reading scene" << std::endl;
	std::cout << "   file: " << path << std::endl;
	std::cout << "   loading ... ";
#endif // DEBUG
	std::ifstream ifs(path);
	std::string config_file((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	// parse json
#ifdef DEBUG
	std::cout << "DONE" << std::endl;
#endif // DEBUG

#ifdef DEBUG
	std::cout << "   parsing ... ";
#endif // DEBUG
	rapidjson::Document config;
	config.Parse(config_file.c_str());
#ifdef DEBUG
	std::cout << "DONE" << std::endl;
#endif // DEBUG

	// construct camera
	// FIXME ugly code

#ifdef DEBUG
	std::cout << "   constructing PipelineType ... ";
#endif // DEBUG
	// pipeline type
	std::string pipeline_type_str = config["pipeline"].GetString();
	if (pipeline_type_str.compare("DEFERRED")) {
		this->pipeline_type = nTiled_pipeline::PipelineType::Deferred;
	}
	else {
		this->pipeline_type = nTiled_pipeline::PipelineType::Forward;
	}
#ifdef DEBUG
	std::cout << "DONE" << std::endl;
	std::cout << "      type: " << pipeline_type_str << std::endl;
#endif // DEBUG


#ifdef DEBUG
	std::cout << "   constructing Camera ... ";
#endif // DEBUG
	// camera control
	std::string camera_control_string = config["camera"]["control"].GetString();

	//TurnTableCameraControl* camera_control_p = new TurnTableCameraControl();

	if (camera_control_string.compare("TURNTABLE") == 0) {
		this->camera_control = new TurnTableCameraControl();
	}
	else {
		this->camera_control = new TurnTableCameraControl();
	}

	// viewport
	auto& viewport_json = config["viewport"];
	this->viewport = glm::uvec2(viewport_json["width"].GetUint(),
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

	this->camera = Camera(this->camera_control,
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


	// construct world
	this->world = nTiled_world::World();


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
		parseGeometry(geometry_path, this->world);
	}


#ifdef DEBUG
	std::cout << "   constructing Light: " << std::endl;
#endif // DEBUG
	// Load lights
	nTiled_world::PointLightConstructor light_constructor = 
		nTiled_world::PointLightConstructor(this->world);
	
	auto& lights_array_json = config["lights"];
	for (rapidjson::Value::ConstValueIterator itr = lights_array_json.Begin(); 
	     itr != lights_array_json.End(); 
		 ++itr) {
		std::string lights_path = (*itr)["path"].GetString();
		parseLights(lights_path, light_constructor);
	}
}

State::~State() {
	// Delete camera control
	delete this->camera_control;

	// Delete stored shaders
	for (const auto& shader_pair : this->shader_catalog) {
		nTiled_pipeline::ShaderBatch* s = shader_pair.second;
		delete s;
	}
}