#include "pipeline\Pipeline.h"

using namespace nTiled_pipeline;

// ---------------------------------------------------------------------------
//  Pipeline
// ---------------------------------------------------------------------------
Pipeline::Pipeline(Camera& camera) : camera(&camera) {}

Camera Pipeline::getActiveCamera() {
	return *(this->camera);
}

void Pipeline::setActiveCamera(Camera& camera) {
	this->camera = &camera;
}