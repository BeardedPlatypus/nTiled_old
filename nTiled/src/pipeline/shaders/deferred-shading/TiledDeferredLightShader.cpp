#include "pipeline\shaders\deferred-shading\DeferredShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------
namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  TiledDeferredLightShader Constructor
// ----------------------------------------------------------------------------
TiledDeferredLightShader::TiledDeferredLightShader(
  DeferredShaderId shader_id,
  const std::string& path_geometry_pass_vertex_shader,
  const std::string& path_geometry_pass_fragment_shader,
  const std::string& path_light_pass_vertex_shader,
  const std::string& path_light_pass_fragment_shader,
  const glm::uvec2& tile_size,
  const world::World& world,
  const state::View& view
  ) :
  DeferredShader(
    shader_id,
    path_geometry_pass_vertex_shader,
    path_geometry_pass_fragment_shader,
    path_light_pass_vertex_shader,
    path_light_pass_fragment_shader,
    world,
    view),
  projector(BoxProjector()),
  light_manager(TiledLightManager(world, view,
                                  tile_size.x, tile_size.y,
                                  projector)) {
}


// ----------------------------------------------------------------------------
//  init
// ----------------------------------------------------------------------------
void TiledDeferredLightShader::init() {
  // Call parent class init
  // ----------------------
  DeferredShader::init();

  glm::uvec2 tileSize = glm::uvec2(this->light_manager.light_grid.tile_width,
                                   this->light_manager.light_grid.tile_height);

  GLint p_tileSize = glGetUniformLocation(this->light_pass_sp, "tileSize");

  GLuint n_tiles_x = this->light_manager.light_grid.n_x;

  GLint p_n_tiles_x = glGetUniformLocation(this->light_pass_sp,
                                           "n_tiles_x");

  glUseProgram(this->light_pass_sp);

  // Load variables into memory
  glUniform2uiv(p_tileSize, 1, glm::value_ptr(tileSize));
  glUniform1ui(p_n_tiles_x, n_tiles_x);

  // Construct Shader Storage Buffer Objects
  GLuint ssbo_handles[2];

  glGenBuffers(2, ssbo_handles);
  this->light_grid_buffer = ssbo_handles[0];
  this->light_index_buffer = ssbo_handles[1];

  // Construct LightGrid buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_grid_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(this->light_manager.light_grid.grid[0]) *
               this->light_manager.light_grid.n_tiles,
               this->light_manager.light_grid.grid,
               GL_DYNAMIC_DRAW);

  // Link LightGrid buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, light_grid_buffer);

  // Construct LightIndex buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->light_manager.light_grid.light_index_list.size(),
               this->light_manager.light_grid.light_index_list.data(),
               GL_DYNAMIC_DRAW);

  // Link LightIndex buffer
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_index_buffer);

  // remove link
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glUseProgram(0);
}

// ----------------------------------------------------------------------------
//  Render functions
// ----------------------------------------------------------------------------
void TiledDeferredLightShader::renderLightPass() {
  // use light pass shader
  glUseProgram(this->light_pass_sp);

  // bind the texture for reading
  this->gBuffer.bindForReading();

  // Update light positions
  glm::mat4 lookAt = this->view.camera.getLookAt();
  glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
  for (GLuint i = 0; i < this->light_data.size(); i++) {
    glm::vec4 lightCameraCoordinates =
      lookAt * this->light_data[i].positionCoordinates;

    glBufferSubData(GL_UNIFORM_BUFFER,
                    sizeof(this->light_data[0]) * i,
                    sizeof(lightCameraCoordinates),
                    glm::value_ptr(lightCameraCoordinates));
  }

  // construct the light grid of tiled shading
  this->light_manager.constructGridFrame();

  // load the tiled datastructures into memory
  // Load LightGridBuffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_grid_buffer);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER,
                  0,
                  sizeof(this->light_manager.light_grid.grid[0]) *
                  this->light_manager.light_grid.n_tiles,
                  this->light_manager.light_grid.grid);
  // Load LightIndexBuffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_index_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(GLuint) * this->light_manager.light_grid.light_index_list.size(),
               this->light_manager.light_grid.light_index_list.data(),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Render elements
  glBindVertexArray(this->fullscreen_quad.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->fullscreen_quad.element_buffer);
  glDrawElements(GL_TRIANGLES,
                 this->fullscreen_quad.element_buffer_size,
                 GL_UNSIGNED_SHORT, 0);
  glUseProgram(0);
}

} // pipeline
} // nTiled