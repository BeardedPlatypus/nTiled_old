#include "world\LightConstructor.h"

// Define sizes of the core light volumes, for representation / debugging purposes
#define CORE_SIZE 0.1
#define CORE_SIZE_SCALE_MAT glm::mat4(CORE_SIZE,       0.0,       0.0, 0.0,\
                                            0.0, CORE_SIZE,       0.0, 0.0,\
										    0.0,       0.0, CORE_SIZE, 0.0,\
                                            0.0,       0.0,       0.0, 1.0)

using namespace nTiled_world;

PointLightConstructor::PointLightConstructor(World& world) : 
	world(world), 
	icosphere_constructor(IcosphereConstructor(world)) {}

void PointLightConstructor::add(std::string name,
	                            glm::vec3 intensity,
	                            float cutoff_radius,
	                            bool is_emitting,
	                            std::string light_volume_shader_id,
	                            glm::mat4 transformationMatrix) {
	glm::mat4 core_transform = transformationMatrix * CORE_SIZE_SCALE_MAT;
	
	glm::mat4 volume_transform = transformationMatrix *
		glm::mat4(cutoff_radius,           0.0,           0.0, 0.0,
			                0.0, cutoff_radius,           0.0, 0.0,
			                0.0,           0.0, cutoff_radius, 0.0,
			                0.0,           0.0,           0.0, 1.0);

	Object* core_p = this->icosphere_constructor.add(name + "_core", 
		                                             light_volume_shader_id,
		                                             core_transform);
	Object* volume_p = this->icosphere_constructor.add(name + "_volume",
		                                               light_volume_shader_id,
		                                               volume_transform);

	glm::vec4 position = transformationMatrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	this->world.addLight(PointLight(position,
		                            intensity, 
		                            cutoff_radius, 
		                            is_emitting, 
		                            *core_p,
		                            *volume_p));
}