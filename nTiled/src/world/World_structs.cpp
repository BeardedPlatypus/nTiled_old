#include "world\World.h"

using namespace nTiled_world;
// ----------------------------------------------------------------------------
//  Mesh
// ----------------------------------------------------------------------------
Mesh::Mesh(std::vector<glm::vec4> vertices,
    	   std::vector<glm::vec3> normals,
		   std::vector<glm::vec3> uvs,
		   std::vector<glm::tvec3<glm::u32>> elements) : vertices(vertices),
		                                                 normals(normals),
		                                                 uvs(uvs),
		                                                 elements(elements) {}

// ----------------------------------------------------------------------------
//  Object
// ----------------------------------------------------------------------------
Object::Object(std::string name,
 	           Mesh mesh,
	           glm::mat4 transformationMatrix,
			   nTiled_pipeline::ShaderId shader_id,
	           bool is_rendered) :
	name(name),
	mesh(mesh),
	shader_id(shader_id),
	transformationMatrix(transformationMatrix),
	is_rendered(is_rendered) {}


Object::Object(std::string name,
	           Mesh mesh,
			   nTiled_pipeline::ShaderId shader_id,
	           glm::mat4 transformationMatrix) :
	Object(name, mesh, transformationMatrix, shader_id, true) {}

// ----------------------------------------------------------------------------
//  World
// ----------------------------------------------------------------------------
World::World() : World(std::vector<Object>(),
	                   std::vector<Mesh>()) {}

World::World(std::vector<Object> objects,
	         std::vector<Mesh> meshes) : objects(objects), 
                                         mesh_catalog(meshes) {}

// ----------------------------------------------------------------------------
int World::addObject(Object object) {
	this->objects.push_back(object);
	return this->objects.size() - 1;
}

Object* World::getObjectPointer(int i) {
	return &(this->objects[i]);
}


int World::addMesh(Mesh mesh) {
	this->mesh_catalog.push_back(mesh);
	return (mesh_catalog.size() - 1);
}

Mesh* World::getMeshPointer(int i) {
	return &(this->mesh_catalog[i]);
}

int World::addLight(PointLight light) {
	this->lights.push_back(light);
	return (this->lights.size() - 1);
}

PointLight* World::getLightPointer(int i) {
	return &(this->lights[i]);
}

