#include "pipeline\PipelineManager.h"

using namespace nTiled_pipeline;
using namespace nTiled_world;
// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
PipelineManager::PipelineManager(Pipeline& pipeline, World& world) : 
	world(&world),
	pipeline(&pipeline) {}

// ----------------------------------------------------------------------------
//  Management Functions
// ----------------------------------------------------------------------------
void PipelineManager::init() {
	// Initialise pipeline
	// ---------------------------------------------------------------------------
	this->pipeline->init(*(this->world));
	
	// Set up face culling
	// ---------------------------------------------------------------------------
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Set up depth buffer
	// ---------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}

void PipelineManager::render() {
	this->pipeline->render();
}