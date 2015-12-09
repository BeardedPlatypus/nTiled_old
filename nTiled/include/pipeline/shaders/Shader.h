#pragma once

#include <glad\glad.h>

#include <vector>

#include "camera\Camera.h"
#include "pipeline\PipelineObject.h"
#include "world\Lights.h"


namespace nTiled_pipeline {
	class ShaderBatch {
	public:
		virtual ~ShaderBatch() {}
		virtual std::string getName() = 0;

		virtual void init(Camera& camera) = 0;
		virtual void render(Camera& camera) = 0;

		virtual void addObject(PipelineObject* obj_p) = 0;
		virtual void addLight(nTiled_world::PointLight* light) = 0;
	};
}