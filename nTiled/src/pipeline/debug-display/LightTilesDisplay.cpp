#include "pipeline\debug-display\LightTilesDisplay.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"

#define VERT_SHADER_PATH_TILE std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/tile/tile.vert")
#define FRAG_SHADER_PATH_TILE std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/tile/tile.frag")

#define VERT_SHADER_PATH_GRID std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/grid/grid.vert")
#define FRAG_SHADER_PATH_GRID std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/grid/grid.frag")

namespace nTiled {
namespace pipeline {

// Public functions
// ----------------------------------------------------------------------------
LightTilesDisplay::LightTilesDisplay(TiledLightManager& manager) :
  light_manager(manager) {
  // load shader program

  // Quad shader
  // -----------------------------------------------------------------
  // Vertex Shader
  std::stringstream vertexShaderBufferTile =
    readShader(VERT_SHADER_PATH_TILE);
  GLuint vertexShaderTile = compileShader(GL_VERTEX_SHADER,
                                          vertexShaderBufferTile.str());

  // Fragment Shader
  std::stringstream fragmentShaderBufferTile =
    readShader(FRAG_SHADER_PATH_TILE);

  GLuint fragmentShaderTile = compileShader(GL_FRAGMENT_SHADER,
                                            fragmentShaderBufferTile.str());
  this->tile_shader_program = createProgram(vertexShaderTile, fragmentShaderTile);

  // Grid shader
  // ------------------------------------------------------------------
  std::stringstream vertexShaderBufferGrid =
    readShader(VERT_SHADER_PATH_GRID);
  GLuint vertexShaderGrid = compileShader(GL_VERTEX_SHADER,
                                          vertexShaderBufferGrid.str());

  // Fragment Shader
  std::stringstream fragmentShaderBufferGrid =
    readShader(FRAG_SHADER_PATH_GRID);

  GLuint fragmentShaderGrid = compileShader(GL_FRAGMENT_SHADER,
                                            fragmentShaderBufferGrid.str());
  this->grid_shader_program = createProgram(vertexShaderGrid, fragmentShaderGrid);

  // Objects 
  // ------------------------------------------------------------------
  float ndc_x_tile = (float)light_manager.light_grid.tile_width /
    (float)light_manager.light_grid.total_width * 2;
  float ndc_y_tile = (float)light_manager.light_grid.tile_height /
    (float)light_manager.light_grid.total_height * 2;

  // Quad objects
  // ------------------------------------------------------------------
  // calculate ndc_width

  for (int y_cursor = 0; y_cursor < light_manager.light_grid.n_y; y_cursor++) {
    for (int x_cursor = 0; x_cursor < light_manager.light_grid.n_x; x_cursor++) {
      // construct quad
      float left = x_cursor * ndc_x_tile - 1.0f;
      float right = (x_cursor + 1) * ndc_x_tile - 1.0f;

      float bottom = (y_cursor)* ndc_y_tile - 1.0f;
      float top = (y_cursor + 1) * ndc_y_tile - 1.0f;

      // vertex positions
      GLfloat quad_vertices[] = {
        left,  bottom, -0.8f,
        right, bottom, -0.8f,
        right, top,    -0.8f,
        left,  top,    -0.8f
      };

      // elements

      GLushort elements[] = {
        0, 1, 2,
        0, 2, 3
      };

      // setup buffers
      GLuint vbo_handles[2];
      glGenBuffers(2, vbo_handles);

      GLuint position_buffer = vbo_handles[0];
      GLuint element_buffer = vbo_handles[1];

      // setup vertex array object
      GLuint vao;
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);

      // set up position buffer
      glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
      glBufferData(GL_ARRAY_BUFFER,
                   12 * sizeof(GLfloat),
                   quad_vertices,
                   GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // set up element buffer
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   6 * sizeof(GLushort),
                   elements,
                   GL_STATIC_DRAW);
      glVertexAttribPointer(1, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, NULL);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      //glBindVertexArray(0);

      this->tiles.push_back(PipelineObject(vao,
                                           element_buffer,
                                           6,
                                           glm::mat4(1.0f)));
    }
  }

  // Grid Object
  // ------------------------------------------------------------------
  int n_grid_lines = light_manager.light_grid.n_x +
    light_manager.light_grid.n_y - 2;
  int n_grid_vertices = 3 * 2 * n_grid_lines;

  GLfloat* grid_vertices = new GLfloat[n_grid_vertices];

  int n_grid_elements = 2 * n_grid_lines;
  GLushort* grid_elements = new GLushort[n_grid_elements];

  GLfloat* grid_vertices_horizontal = grid_vertices;
  GLfloat* grid_vertices_vertical = &grid_vertices[3 * n_grid_lines];

  GLushort* grid_elements_horizontal = grid_elements;
  GLushort* grid_elements_vertical = &grid_elements[n_grid_lines];

  // horizontal lines
  for (int i = 0; i < light_manager.light_grid.n_y - 1; i++) {
    // left point
    grid_vertices_horizontal[6 * i + 0] = -1.0f;
    grid_vertices_horizontal[6 * i + 1] = (i + 1) * ndc_y_tile - 1;
    grid_vertices_horizontal[6 * i + 2] = -.8f;
    // right point
    grid_vertices_horizontal[6 * i + 3] = 1.0f;
    grid_vertices_horizontal[6 * i + 4] = (i + 1) * ndc_y_tile - 1;
    grid_vertices_horizontal[6 * i + 5] = -.8f;

    // elements
    grid_elements_horizontal[2 * i + 0] = 2 * i + 0;
    grid_elements_horizontal[2 * i + 1] = 2 * i + 1;
  }

  // vertical lines
  for (int i = 0; i < light_manager.light_grid.n_x - 1; i++) {
    // top point
    grid_vertices_vertical[6 * i + 0] = (i + 1) * ndc_x_tile - 1;
    grid_vertices_vertical[6 * i + 1] = 1.0f;
    grid_vertices_vertical[6 * i + 2] = -.8f;
    // bottom point
    grid_vertices_vertical[6 * i + 3] = (i + 1) * ndc_x_tile - 1;
    grid_vertices_vertical[6 * i + 4] = -1.0f;
    grid_vertices_vertical[6 * i + 5] = -.8f;

    // elements
    grid_elements_vertical[2 * i + 0] = 2 * i + 0 + n_grid_lines;
    grid_elements_vertical[2 * i + 1] = 2 * i + 1 + n_grid_lines;
  }

  GLuint vbo_handles[2];
  glGenBuffers(2, vbo_handles);

  GLuint position_buffer = vbo_handles[0];
  GLuint element_buffer = vbo_handles[1];

  // setup vertex array buffer grid objects
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // position buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               n_grid_vertices * sizeof(GLfloat),
               grid_vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               n_grid_elements * sizeof(GLushort),
               grid_elements,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // add to this LightTilesDisplay
  this->grid = PipelineObject(vao,
                              element_buffer,
                              n_grid_elements,
                              glm::mat4(1.0f));

  // remove allocated vertices from dynamic memory
  delete[] grid_vertices;
  delete[] grid_elements;
}

void LightTilesDisplay::render() {
  this->drawTiles();
  this->drawGrid();
}

// Private functions
// ----------------------------------------------------------------------------
void LightTilesDisplay::drawGrid() {
  glUseProgram(this->grid_shader_program);

  glBindVertexArray(this->grid.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->grid.element_buffer);
  glDrawElements(GL_LINES,
                 this->grid.element_buffer_size,
                 GL_UNSIGNED_SHORT, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

void LightTilesDisplay::drawTiles() {
  glUseProgram(this->tile_shader_program);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int max_lights = 0;
  int n_light_tile = 0;
  for (int i = 0; i < this->light_manager.light_grid.n_tiles; i++) {
    n_light_tile = this->light_manager.light_grid.grid[i].y;
    if (n_light_tile > max_lights) {
      max_lights = n_light_tile;
    }
  }

  if (max_lights == 0) {
    max_lights = 1;
  }

  int cursor = 0;
  for (PipelineObject obj : this->tiles) {
    GLfloat fraction = (float)this->light_manager.light_grid.grid[cursor].y /
      (float)max_lights;

    GLint p_colour_fraction = glGetUniformLocation(this->tile_shader_program,
                                                   "colour_intensity");

    glUniform1f(p_colour_fraction, fraction);

    // calculate colour of the tile
    // bind vertex array object
    glBindVertexArray(obj.vao);

    // render quad
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 obj.element_buffer);
    glDrawElements(GL_TRIANGLES,
                   obj.element_buffer_size,
                   GL_UNSIGNED_SHORT, 0);
    cursor++;
  }
  glBindVertexArray(0);
  glUseProgram(0);
  glDisable(GL_BLEND);
}

} // pipeline
} // nTiled