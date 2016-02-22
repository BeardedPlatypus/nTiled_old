#include "world\ObjectConstructors.h"

using namespace nTiled_world;

CubeConstructor::CubeConstructor(World& world) : world(world) {
	// construct cube mesh
	std::vector<glm::vec4> vertices_cube = {
		glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f),

		glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f),

		glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f),
		glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f),
		glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f)
	};

	std::vector<glm::vec3> normals_cube = {
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),

		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),

		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f),

		glm::vec3(0.0f,  1.0f,   0.0f),
		glm::vec3(0.0f,  1.0f,   0.0f),
		glm::vec3(0.0f,  1.0f,   0.0f),
		glm::vec3(0.0f,  1.0f,   0.0f),

		glm::vec3(-1.0f,  0.0f,   0.0f),
		glm::vec3(-1.0f,  0.0f,   0.0f),
		glm::vec3(-1.0f,  0.0f,   0.0f),
		glm::vec3(-1.0f,  0.0f,   0.0f),

		glm::vec3(1.0f,  0.0f,   0.0f),
		glm::vec3(1.0f,  0.0f,   0.0f),
		glm::vec3(1.0f,  0.0f,   0.0f),
		glm::vec3(1.0f,  0.0f,   0.0f)
	};

	std::vector<glm::tvec3<glm::u32>> elements_cube = {
		glm::tvec3<glm::u32>( 2,  4,  3),
		glm::tvec3<glm::u32>( 1,  2,  3),
		glm::tvec3<glm::u32>( 6,  5,  7),
		glm::tvec3<glm::u32>( 8,  6,  7),
		glm::tvec3<glm::u32>(12, 11,  9),
		glm::tvec3<glm::u32>(10, 12,  9),
		glm::tvec3<glm::u32>(16, 14, 13),
		glm::tvec3<glm::u32>(15, 16, 13),
		glm::tvec3<glm::u32>(20, 19, 17),
		glm::tvec3<glm::u32>(18, 20, 17),
		glm::tvec3<glm::u32>(22, 21, 23),
		glm::tvec3<glm::u32>(24, 22, 23)
	};

	int i = this->world.addMesh(Mesh(vertices_cube,
		                             normals_cube,
		                             std::vector<glm::vec3>(),
		                             elements_cube));
	this->cube = this->world.getMeshPointer(i);
}

// ----------------------------------------------------------------------------
//  Object construction methods.
Object* CubeConstructor::add(std::string name,
	                         nTiled_pipeline::ShaderId shader_id,
	                         glm::mat4 transformationMatrix) {
	int i = this->world.addObject(Object(name, 
		                          *(this->cube), 
		                          shader_id,
		                          transformationMatrix));
	return this->world.getObjectPointer(i);
}
