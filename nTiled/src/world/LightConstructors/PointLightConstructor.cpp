#include "world\LightConstructor.h"

#define CORE_PATH "C:/Users/Monthy/Documents/projects/thesis/scenes/debug/light/light_core.obj"
#define RADIUS_PATH "C:/Users/Monthy/Documents/projects/thesis/scenes/debug/light/light_radius_b.obj"

using namespace nTiled_world;

PointLightConstructor::PointLightConstructor(World& world) : 
	world(world), 
	core_constructor(AssImpConstructor(world,
		                               CORE_PATH)),
	cutoff_constructor(AssImpConstructor(world,
	                                     RADIUS_PATH)) {}

void PointLightConstructor::add(std::string name,
	                            glm::vec3 intensity,
	                            float cutoff_radius,
	                            bool is_emitting,
	                            glm::mat4 transformationMatrix) {
	glm::mat4 cutoff_transform = transformationMatrix *
		glm::mat4(cutoff_radius,           0.0,           0.0, 0.0,
			                0.0, cutoff_radius,           0.0, 0.0,
			                0.0,           0.0, cutoff_radius, 0.0,
			                0.0,           0.0,           0.0, 1.0);

	/*
	Object* core_p = this->core_constructor.add(
		name + "_core",
		nTiled_pipeline::ShaderId::ForwardDebugCoreLight,
		transformationMatrix);
		*/
	Object* cutoff_p = this->cutoff_constructor.add(
		name + "_cutoff",
		nTiled_pipeline::ShaderId::ForwardDebugCutOffLight,
		transformationMatrix);

	glm::vec4 position = transformationMatrix * glm::vec4(0.0, 0.0, 0.0, 1.0);
	this->world.addLight(PointLight(position,
		                            intensity, 
		                            cutoff_radius, 
		                            is_emitting, 
		                            *cutoff_p, //*core_p,
		                            *cutoff_p));
}