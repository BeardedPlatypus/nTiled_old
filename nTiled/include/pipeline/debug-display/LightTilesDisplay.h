#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <vector>
#include <glm\glm.hpp>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\debug-display\DebugShader.h"
#include "pipeline\PipelineObject.h"

#include "pipeline\light-management\Tiled\TiledLightManager.h"


namespace nTiled {
namespace pipeline {

class LightTilesDisplay : public DebugShader {
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

} // pipeline
} // nTiled
