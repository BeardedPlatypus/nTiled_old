#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <string>
#include <vector>

#include <glad\glad.h>
#include <glm\glm.hpp>

namespace nTiled_world {
	// ------------------------------------------------------------------------
	//  World structs
	// ------------------------------------------------------------------------
	struct Mesh {
	public:
		Mesh(std::vector<glm::vec4> vertices,
		 	 std::vector<glm::vec3> normals,
			 std::vector<glm::vec3> uvs,
			 std::vector<glm::tvec3<glm::u32>> elements);

		const std::vector<glm::vec4> vertices;
		const std::vector<glm::vec3> normals;
		const std::vector<glm::vec3> uvs;
		const std::vector<glm::tvec3<glm::u32>> elements;

	private:
		Mesh();
	};

	// ------------------------------------------------------------------------
	struct Object {
	public:
		Object(std::string name,
			   nTiled_world::Mesh& mesh);

		const std::string name;
		const nTiled_world::Mesh& mesh;
	private:
		Object();
	};

	// ------------------------------------------------------------------------
	struct World {
	public:
		World(std::vector<Object*> objects);

		const std::vector<Object*> objects;
	};

	// -----------------------------------------------------------------------
	//  Read functions
	// -----------------------------------------------------------------------
	void objectsFromOBJ(std::string &path,
		                std::vector<nTiled_world::Object*> &objects);

}