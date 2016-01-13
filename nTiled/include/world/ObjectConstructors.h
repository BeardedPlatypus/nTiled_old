#pragma once
#include "world\World.h"

namespace nTiled_world {
	// ------------------------------------------------------------------------
	//  Abstract class
	class PrimitiveConstructor {
	public:
		virtual ~PrimitiveConstructor() {}
		virtual Object* add(std::string name,
			                std::string shader_id,
			                glm::mat4 transformationMatrix) = 0;
	};

	// ------------------------------------------------------------------------
	//  Cube meshes
	class CubeConstructor : public PrimitiveConstructor {
	public:
		CubeConstructor(World& world);
		Object* add(std::string name, 
			        std::string shader_id,
			        glm::mat4 transformationMatrix);

	private:
		World& world;
		Mesh* cube;
	};

	// ------------------------------------------------------------------------
	//  Obj Mesh loader
	class ObjConstructor : public PrimitiveConstructor {
	public: 
		ObjConstructor(World& world, std::string path);
		Object* add(std::string name,
			        std::string shader_id,
			        glm::mat4 transformationMatrix);
	private:
		World& world;
		Mesh* obj_mesh;
	};

	
	class AssImpConstructor : public PrimitiveConstructor {
	public:
		AssImpConstructor(World& world, std::string path);
		Object* add(std::string name,
			        std::string shader_id,
			        glm::mat4 transformationMatrix);
	private:
		World& world;
		Mesh* obj_mesh;
	};


	// ------------------------------------------------------------------------
	//  Icosphere loader
	class IcosphereConstructor : public PrimitiveConstructor {
	public:
		IcosphereConstructor(World& world);
		Object* add(std::string name, 
			        std::string shader_id,
			        glm::mat4 transformationMatrix);
	private:
		World& world;
		Mesh* icosphere;
	};
}