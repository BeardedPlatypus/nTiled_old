#include "pipeline\shaders\forward-shading\ForwardShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include "pipeline\shader-util\LoadShaders.h"

namespace nTiled {
namespace pipeline {
// ----------------------------------------------------------------------------
//  BasicForwardTiledLightingShader
// ----------------------------------------------------------------------------
ForwardTiledShader::ForwardTiledShader(ForwardShaderId shader_id,
                                       const std::string& path_vertex_shader,
                                       const std::string& path_fragment_shader,
                                       glm::uvec2 tile_size,
                                       const world::World& world,
                                       const state::View& view) :
  ForwardShader(shader_id,
                path_vertex_shader,
                path_fragment_shader),
  projector(BoxProjector()),
  light_manager(TiledLightManager(world, view,
                                  tile_size.x, tile_size.y,
                                  projector)) {
}

void ForwardTiledShader::init(const camera::Camera& camera) {
  // Call Parent class init
  // ------------------------------------------------------------------------
  ForwardShader::init(camera);

  // Tiled Specific init
  // ------------------------------------------------------------------------
  // tilesize
  glm::uvec2 tileSize = glm::uvec2(this->light_manager.light_grid.tile_width,
                                   this->light_manager.light_grid.tile_height);

  GLint p_tileSize = glGetUniformLocation(this->shader_program, "tileSize");

  GLuint n_tiles_x = this->light_manager.light_grid.n_x;

  GLint p_n_tiles_x = glGetUniformLocation(this->shader_program,
                                           "n_tiles_x");

  glUseProgram(this->shader_program);

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

void ForwardTiledShader::render(const camera::Camera& camera) {
  // start program
  glUseProgram(this->shader_program);

  // Get the lookAt matrix
  glm::mat4 lookAt = camera.getLookAt();

  // construct the light grid of tiled shading
  this->light_manager.constructGridFrame();

  // load the tiled datastructures into memory
  // Load LightGridBuffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->light_grid_buffer);
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

  // Render each model associated with this shader
  for (PipelineObject obj : this->objects) {
    glBindVertexArray(obj.vao);
    // retrieve memory locations shared per model
    GLint p_modelToCamera = glGetUniformLocation(
      this->shader_program,
      "modelToCameraMatrix");

    // calculate transformation matrices per model
    glm::mat4 modelToCamera = lookAt * obj.transformation;
    glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
                       glm::value_ptr(modelToCamera));

    // Update light data
    glm::mat4 worldToModelSpace = glm::inverse(obj.transformation);

    glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
    for (GLuint i = 0; i < this->light_data.size(); i++) {
      glm::vec4 lightModelCoordinates =
        worldToModelSpace * this->light_data[i].positionCoordinates;
      glBufferSubData(
        GL_UNIFORM_BUFFER,
        sizeof(this->light_data[0]) * i,
        sizeof(lightModelCoordinates),
        glm::value_ptr(lightModelCoordinates)
        );
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // render the elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 obj.element_buffer);
    glDrawElements(GL_TRIANGLES,
                   obj.element_buffer_size,
                   GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}

void ForwardTiledShader::loadShaders() {
  // Vertex Shader
  // -----------------------------------------------------------------
  std::stringstream vertexShaderBuffer =
    readShader(this->path_vertex_shader);
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
                                      vertexShaderBuffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream fragmentShaderBuffer;

  std::string replaceLine = "#define NUM_LIGHTS ";
  std::string n_lights = std::to_string(this->light_data.size());

  std::ifstream file_in(this->path_fragment_shader, std::ifstream::in);

  for (std::string line; std::getline(file_in, line);) {
    if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
      fragmentShaderBuffer << replaceLine << n_lights << std::endl;
    } else {
      fragmentShaderBuffer << line << std::endl;
    }
  }

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
                                        fragmentShaderBuffer.str());

  this->shader_program = createProgram(vertexShader, fragmentShader);
}

} // pipeline
} // nTiled
