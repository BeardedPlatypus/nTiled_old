#pragma once

#include "state\State.h"
#include "pipeline\Pipeline.h"

namespace nTiled_pipeline {
	class PipelineManager {
	public:
		PipelineManager(nTiled_state::State& state);
		~PipelineManager();


		void init();
		void render();
	private:
		Pipeline* pipeline;
		nTiled_state::State& state;
	};
}