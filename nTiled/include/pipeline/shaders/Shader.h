#pragma once

#include <glad\glad.h>

#include <vector>

#include "camera\Camera.h"
#include "pipeline\PipelineObject.h"
#include "world\Lights.h"

#include "pipeline\shaders\ShaderDefinitions.h"

// TODO add possible to change state to a different object
namespace nTiled_pipeline {
	class ShaderBatch {
	public:
		virtual ~ShaderBatch() {}
		virtual ShaderId getId() = 0;

		virtual void init(Camera& camera) = 0;
		virtual void render(Camera& camera) = 0;

		virtual void addObject(nTiled_world::Object& object) = 0;
		virtual void addLight(nTiled_world::PointLight* light) = 0;
	};
}