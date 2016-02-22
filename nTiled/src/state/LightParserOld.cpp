#include "state\State.h"

#include <fstream>
#include <cstdlib>

#include <glm/gtc/matrix_transform.hpp>

#define BUFFER_SIZE 6 // support 10000 lights in theory

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif // DEBUG


void nTiled_state::parseLights(std::string path,  
	                           nTiled_world::LightConstructor& pointLightConstructor) {
	// load file 
	// TODO: inspect whether this is a necessary bottleneck to fix
#ifdef DEBUG
		std::cout << "      file: " << path << std::endl;
#endif // DEBUG
	std::ifstream file(path);
	if (file.is_open()) {
		// parse lights
		std::string line;
		int i = 0;
		while (std::getline(file, line)) {
			if (!line.length()) continue; // skip empty line
			if (line[0] == '#') continue; // ignore comment lines
			if (line[0] == 'p') {         // point light
				char *ptr; 

				// read values
				double x = strtod(line.c_str() + 2, &ptr);
				double y = strtod(ptr + 1, &ptr);
				double z = strtod(ptr + 1, &ptr);

				double r = strtod(ptr + 1, &ptr);
				double g = strtod(ptr + 1, &ptr);
				double b = strtod(ptr + 1, &ptr);

				double radius = strtod(ptr + 1, &ptr);

				// create name
				char buffer[BUFFER_SIZE];
				int n = sprintf_s(buffer, "p%d", i);

				// add light to world
				pointLightConstructor.add(std::string(buffer),
					                      glm::vec3(r, g, b),
					                      radius, 
					                      true,
					                      glm::translate(glm::mat4(1.0f), 
											             glm::vec3(x, y, z)));
				i++;
			}
		}
#ifdef DEBUG
		std::cout << "         lights read: " << i << std::endl;
#endif // DEBUG

	}
}
