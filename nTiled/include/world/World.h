#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include "world\Object.h"
#include "world\Lights.h"

#include <string>
#include <vector>

namespace nTiled_world {
	struct World {
	public:
		World();
		World(std::vector<Object> objects,
			  std::vector<Mesh> meshes);

		std::vector<Object> objects;
		std::vector<PointLight> lights;

		// ------------------------------
		int addObject(Object obj);
		Object* getObjectPointer(int i);
		// ------------------------------
		int addMesh(Mesh mesh);
		Mesh* getMeshPointer(int i);
		// ------------------------------
		int addLight(PointLight light);
		PointLight* getLightPointer(int i);

	private:
		std::vector<Mesh> mesh_catalog;
	};
}