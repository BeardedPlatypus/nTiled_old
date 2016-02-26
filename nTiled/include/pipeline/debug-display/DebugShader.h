#pragma once

namespace nTiled_pipeline {
	class DebugShader {
	public:
		virtual ~DebugShader() {}

		virtual void render() = 0;
	};
}