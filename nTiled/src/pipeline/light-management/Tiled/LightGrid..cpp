#include "pipeline\light-management\Tiled\LightGrid.h"

// ----------------------------------------------------------------------------
//  System Libraries
// ----------------------------------------------------------------------------
#include <algorithm>

// ----------------------------------------------------------------------------
using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  LightGrid class
// ----------------------------------------------------------------------------
// constructor
LightGrid::LightGrid(int total_width, int total_height,
       	             int tile_width, int tile_height) :
	total_width(total_width), total_height(total_height),
	tile_width(tile_width), tile_height(tile_height),
	light_index_list(std::vector<int>()) {
	// calc number tiles x
	this->n_x = total_width / tile_width;
	if (tile_width * n_x < total_width) {
		this->n_x++;
	}
	// calc number tiles y
	this->n_y = total_height / tile_height;
	if (tile_height * n_y < total_height) {
		this->n_y++;
	}

	this->n_tiles = n_x * n_y;
	// allocate memory for the tiles
	this->grid = new glm::uvec2[this->n_tiles];
	this->light_index_list_raw = new std::vector<int>[this->n_tiles];

	this->clearGrid();
}

// destructor
// ----------------------------------------------------------------------------
LightGrid::~LightGrid() {
	delete[] this->grid;
	delete[] this->light_index_list_raw;
}

//  Grid Construction Methods
// ----------------------------------------------------------------------------
void LightGrid::clearGrid() {
	for (int i = 0; i < n_tiles; i++) {
		this->grid[i] = glm::uvec2(0, 0);
		this->light_index_list_raw[i] = std::vector<int>();
	}
	this->total_light_indices = 0;
}

void LightGrid::incrementTiles(glm::uvec4 tiles, int light_index) {
	// initial position
	unsigned int cursor = this->n_x * tiles.y + tiles.x;
	unsigned int cursor_y = tiles.y;
	unsigned int end_pos = this->n_x * (tiles.w) + tiles.z;

	// calculate total number of indices added
	this->total_light_indices += ((tiles.z - tiles.x) * (tiles.w - tiles.y));
	
	while (cursor <= end_pos) {
		// add index to internal light list
		this->light_index_list_raw[cursor].push_back(light_index);
		cursor++;

		if (cursor > (cursor_y * this->n_x + tiles.z)) {
			cursor_y++;
			cursor = this->n_x * cursor_y + tiles.x;
		}
	}
}

void LightGrid::finaliseGrid() {
	this->light_index_list = std::vector<int>(this->total_light_indices);

	unsigned int current_offset = 0;
	unsigned int n_indices;

	for (int i = 0; i < this->n_tiles; i++) {
		// add elements to light index list
		for (int light_index : this->light_index_list_raw[i]) {
			light_index_list.push_back(light_index);
		}
		// set values in light grid
		n_indices = this->light_index_list_raw[i].size();
		this->grid[i] = glm::uvec2(current_offset,
			                       n_indices);
		current_offset += n_indices;
	}
}