#pragma once

namespace nTiled_pipeline {
	enum class ShaderId {
		// ====================================================================
		//  Forward
		// ====================================================================
		//  Regular Forward Shaders
		// --------------------------------------------------------------------
		ForwardSolid,
		ForwardBasicVertex,
		ForwardBasicFrag,
		ForwardAttenuated,
		ForwardTiled,
		
		// Tiled Forward Shaders
		// --------------------------------------------------------------------


		// Clustered Forward Shaders
		// --------------------------------------------------------------------


		//  Debug Forward Shaders
		// --------------------------------------------------------------------
		ForwardDebugCoreLight,
		ForwardDebugCutOffLight

		// ====================================================================
		//  Deferred
		// ====================================================================
	};
}