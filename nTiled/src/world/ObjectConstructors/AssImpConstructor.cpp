#include "world\ObjectConstructors.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla	

#include <iostream>
	
using namespace nTiled_world;

AssImpConstructor::AssImpConstructor() : world(World()) {}

AssImpConstructor::AssImpConstructor(World& world, std::string path) : world(world) {

	// Import file with AssImp
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace      |
		aiProcess_Triangulate           |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals      |
		aiProcess_SortByPType);

    // Check if read scene contains any meshes
	if (!scene->HasMeshes()) {
		throw std::runtime_error(std::string("Could not detect any meshes in file: ") + path);
	}

	// Convert scene file into appropriate mesh file.
	//   We assume only the first mesh is of interest TODO: fix this
	aiMesh* mesh = scene->mMeshes[0];
	
	std::vector<glm::vec4> vertices = std::vector<glm::vec4>();
	std::vector<glm::vec3> normals = std::vector<glm::vec3>();
	std::vector<glm::vec3> uvs = std::vector<glm::vec3>();

	aiVector3D* mesh_vertices = mesh->mVertices;
	aiVector3D* mesh_normals = mesh->mNormals;
	aiVector3D* mesh_uvs = mesh->mTextureCoords[0];

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		// Vertices
		aiVector3D v = mesh_vertices[i];
		vertices.push_back(glm::vec4(v.x, v.y, v.z, 1.0f));
		// Normals
		aiVector3D n = mesh_normals[i];
		normals.push_back(glm::vec3(n.x, n.y, n.z));

		// UVs
		if (mesh->HasTextureCoords(0)) {
			aiVector3D uv = mesh_uvs[i];
			uvs.push_back(glm::vec3(uv.x, uv.y, uv.z));
		}
	}

	// -- Elements --
	aiFace* mesh_faces = mesh->mFaces;
	std::vector<glm::tvec3<glm::u32>> elements = std::vector<glm::tvec3<glm::u32>>();
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace f = mesh_faces[i];
		elements.push_back(glm::tvec3<glm::u32>(f.mIndices[0],
			                                    f.mIndices[1],
			                                    f.mIndices[2]));
	}

	int i = this->world.addMesh(Mesh(vertices,
		                             normals,
		                             uvs,
		                             elements));
	this->obj_mesh = this->world.getMeshPointer(i);
}

Object* AssImpConstructor::add(std::string name,
	                           nTiled_pipeline::ShaderId shader_id,
	                           glm::mat4 transformationMatrix) {
	int i = this->world.addObject(Object(name,
		                                 *(this->obj_mesh),
		                                 shader_id,
		                                 transformationMatrix));

	return this->world.getObjectPointer(i);
}