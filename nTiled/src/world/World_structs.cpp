#include "world\World.h"

using namespace nTiled_world;
// ----------------------------------------------------------------------------
Mesh::Mesh(std::vector<glm::vec4> vertices,
    	   std::vector<glm::vec3> normals,
		   std::vector<glm::vec3> uvs,
		   std::vector<glm::tvec3<glm::u32>> elements) : vertices(vertices),
		                                                 normals(normals),
		                                                 uvs(uvs),
		                                                 elements(elements) {}

// ----------------------------------------------------------------------------
Object::Object(std::string name,
		       Mesh mesh) : name(name),
		                    mesh(mesh) {}

// ----------------------------------------------------------------------------
World::World() : World(std::vector<Object>(),
	                   std::vector<Mesh>()) {}

World::World(std::vector<Object> objects,
	         std::vector<Mesh> meshes) : objects(objects), 
                                         mesh_catalog(meshes) {}
