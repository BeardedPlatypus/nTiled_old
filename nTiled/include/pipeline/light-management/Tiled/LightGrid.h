#pragma once

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>

namespace nTiled {
namespace pipeline {
// ------------------------------------------------------------------------
//  LightGrid class
// ------------------------------------------------------------------------
/*!
 * LightGrid stores both the light index list and light grid which are
 * constructed by the LightManager.
 * It adds convenience classes for adding lights to
 */
class LightGrid {
  // Data class, with several convenience functions to simplify 
  // interaction
      // Light grid class
 public:
  // Constructor
  LightGrid(int total_width, int total_height,
            int tile_width, int tile_height);
  ~LightGrid();

  // Members
  int total_width;
  int total_height;

  int tile_width;
  int tile_height;

  int n_x;
  int n_y;
  int n_tiles;

  // Construction settings
  void incrementTiles(glm::uvec4 tiles, int light_index);

  /*!
   * Set all grid entries to uvec2(0, 0)
   */
  void clearGrid();

  /*!
   * Set grid to values represented internally
   */
  void finaliseGrid();

  //TODO figure out whether it is necessary to give this 
  // a more specific access function or not
  glm::uvec2* grid;
  std::vector<GLuint> light_index_list;

 private:
  std::vector<GLuint>* light_index_list_raw;
  int total_light_indices;
};

} // pipeline
} // nTiled
