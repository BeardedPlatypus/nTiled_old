#pragma once

#include "world\World.h"
#include "world\ObjectConstructors.h"

namespace nTiled_world {
	// ------------------------------------------------------------------------
	//  Abstract class
	class LightConstructor {
	public:
		virtual ~LightConstructor() {}
		virtual void add(std::string name,
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
		void add(std::string name,
			     glm::vec3 intensity,
			     float cutoff_radius,
			     bool is_emitting,
			     std::string light_volume_shader_id,
			     glm::mat4 transformationMatrix);

	private:
		World& world;
		IcosphereConstructor icosphere_constructor;
	};
}