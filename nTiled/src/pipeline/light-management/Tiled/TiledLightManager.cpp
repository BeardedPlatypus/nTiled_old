#include "pipeline\light-management\Tiled\TiledManager.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  TiledLightManager class
// ----------------------------------------------------------------------------
// constructor 
TiledLightManager::TiledLightManager(
	nTiled_state::State& state,
	int tile_width, int tile_height,
	LightProjector& projector) :
	state(state),
	light_grid(LightGrid(state.viewport.x, state.viewport.y,
		                 tile_width, tile_height)),
	projector(projector) {}

void TiledLightManager::constructGridFrame() {
	// Clear Grid
	this->light_grid.clearGrid();

	// Build new grid with projector
	int index = 0;
	glm::uvec4 affected_tiles;
	for (nTiled_world::PointLight light : this->state.world.lights) {
		if (this->projector.computeProjection(light,
			                                  this->state.camera,
			                                  this->state.viewport,
			                                  glm::uvec2(this->light_grid.tile_width,
				                                         this->light_grid.tile_height),
			                                  affected_tiles)) {
			this->light_grid.incrementTiles(affected_tiles, index);
		}
		index++;
	}

	// Finalise grid
	this->light_grid.finaliseGrid();
}