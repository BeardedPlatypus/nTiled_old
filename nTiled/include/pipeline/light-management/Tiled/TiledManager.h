#pragma once

#include "state\State.h"
#include "pipeline\light-management\Tiled\LightGrid.h"
#include "pipeline\light-management\Tiled\LightProjector.h"

namespace nTiled_pipeline {
	// ------------------------------------------------------------------------
	//  Light Manager class
	// ------------------------------------------------------------------------
	class TiledLightManager {
	public:
		TiledLightManager(
			nTiled_state::State& state,
			int tile_width, int tile_height,
			LightProjector& projector);
        
		// construction method
		void constructGridFrame();

		// data members
		// global list -> use similar data structure on gpu as now
		// currently use static lights, thus lights only need to
		// be loaded once. This could be 
		nTiled_state::State& state;

		/*!
		 * LightGrid datastructure to which this lightmanager writes.
		 */
		LightGrid light_grid;

		/*!
		 * LightProjector to be used to calculate the affected lights
		 */
		LightProjector& projector;
	};
}
