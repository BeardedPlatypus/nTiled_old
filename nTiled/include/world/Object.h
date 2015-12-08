#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

#include <string>
#include <vector>

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
			nTiled_world::Mesh mesh,
			std::string shader_id,
			glm::mat4 transformationMatrix);
		Object(std::string name,
			nTiled_world::Mesh mesh,
			glm::mat4 transformationMatrix,
			std::string shader_id,
			bool is_rendered);


		const std::string name;
		const nTiled_world::Mesh mesh;
		glm::mat4 transformationMatrix;

		std::string shader_id;

		bool is_rendered;
	private:
		Object();
	};
}