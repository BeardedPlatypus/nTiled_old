#pragma once

#include "pipeline\debug-display\DebugShader.h"
#include "pipeline\PipelineObject.h"

#include <vector>
#include <glm\glm.hpp>

#include "pipeline\light-management\Tiled\TiledManager.h"

namespace nTiled_pipeline {
	class LightTilesDisplay: public DebugShader {
	public:
		LightTilesDisplay(TiledLightManager& manager);
		void render();

	private:
		std::vector<PipelineObject> quads;

		GLuint tile_shader_program;
		GLuint grid_shader_program;

		void drawGrid();
		void drawTiles();

		TiledLightManager& light_manager;
		
		std::vector<PipelineObject> tiles;
		PipelineObject grid;
	};
}
