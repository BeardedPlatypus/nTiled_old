#include "world\ObjectConstructors.h"

#include <regex>
#include <fstream>

using namespace nTiled_world;

void indicesToTriangleVector(std::smatch matched_values,
	unsigned short offset_start, unsigned short offset_between,
	glm::tvec3<glm::u32> &result1,
	glm::tvec3<glm::u32> &result2) {

	result1.x = std::stoul(matched_values[1 + offset_start]) - 1;
	result1.y = std::stoul(matched_values[1 + offset_start + offset_between]) - 1;
	result1.z = std::stoul(matched_values[1 + offset_start + 2 * offset_between]) - 1;

	if (matched_values[1 + offset_start + 3 * offset_between] != "") {
		result2.x = std::stoul(matched_values[1 + offset_start]) - 1;
		result2.y = std::stoul(matched_values[1 + offset_start + 2 * offset_between]) - 1;
		result2.z = std::stoul(matched_values[1 + offset_start + 3 * offset_between]) - 1;
	}
}

void faceNormalsToVertexNormals(std::smatch matched_values,
	unsigned short offset_start_vertex,
	unsigned short offset_normal,
	unsigned short offset_between,
	std::vector<glm::vec3> &face_normals,
	std::vector<glm::vec3> &vertex_normals,
	std::vector<unsigned int> &vertex_normals_count) {

	// first vertex normal
	vertex_normals[std::stoul(matched_values[1 + offset_start_vertex]) - 1] +=
		face_normals[
			std::stoul(matched_values[1 + offset_start_vertex + offset_normal]) - 1
		];

	vertex_normals_count[std::stoul(matched_values[1 + offset_start_vertex]) - 1] += 1;

	// second vertex normal
	vertex_normals[std::stoul(matched_values[1 + offset_start_vertex
		+ offset_between]) - 1] +=
		face_normals[std::stoul(matched_values[1 + offset_start_vertex
			+ offset_between
			+ offset_normal]) - 1
		];
	vertex_normals_count[std::stoul(matched_values[1 + offset_start_vertex
		+ offset_between]) - 1] += 1;

	// third vertex normal
	vertex_normals[std::stoul(matched_values[1 + offset_start_vertex
		+ 2 * offset_between]) - 1] +=
		face_normals[std::stoul(matched_values[1 + offset_start_vertex
			+ 2 * offset_between
			+ offset_normal]) - 1
		];
	vertex_normals_count[std::stoul(matched_values[1 + offset_start_vertex
		+ 2 * offset_between]) - 1] += 1;

	// fourth vertex normal if it exist
	if (matched_values[1 + offset_start_vertex + 3 * offset_between] != "") {
		vertex_normals[std::stoul(matched_values[
			1 + offset_start_vertex
				+ 3 * offset_between]) - 1] +=
			face_normals[std::stoul(matched_values[
				1 + offset_start_vertex
					+ 3 * offset_between
					+ offset_normal]) - 1
			];
			vertex_normals_count[std::stoul(matched_values[
				1 + offset_start_vertex
					+ 3 * offset_between]) - 1] += 1;
	}
}

ObjConstructor::ObjConstructor(World& world, std::string path) : world(world) {
	std::ifstream file_in(path, std::ifstream::in);

	// error if file could not be read
	if (!file_in) {
		throw std::runtime_error(std::string("Can't open obj file: ") + path);
	}

	// result vectors
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> uv_coords;
	std::vector<glm::vec3> face_normals;
	std::vector<glm::vec3> vertex_normals;
	std::vector<unsigned int> vertex_normals_count;
	std::vector<glm::tvec3<glm::u32>> elements;

	// regex definitions
	// -----------------------------------------------------------------------
	std::string float_regex = "([+-]?(?:0|(?:[1-9][0-9]*))(?:\.[0-9]*)?)";

	std::regex regex_vertex_locations("^" + float_regex + " +"
		+ float_regex + " +"
		+ float_regex + "(?: +"
		+ float_regex + ")?$");

	std::regex regex_texture_coordinates("^" + float_regex + " +"
		+ float_regex + "(?: +"
		+ float_regex + ")?$");

	std::regex regex_normals("^" + float_regex + " +"
		+ float_regex + " +"
		+ float_regex + "$");

	// -----------------------------------------------------------------------
	std::string index_regex = "(0|(?:[1-9][0-9]*))";
	std::string v_vt_vn_element_regex = index_regex + "/" + index_regex + "/" + index_regex;
	std::string v_vt_element_regex = index_regex + "/" + index_regex;
	std::string v_vn_element_regex = index_regex + "//" + index_regex;

	std::regex regex_face_v_vt_vn("^" + v_vt_vn_element_regex + " +"
		+ v_vt_vn_element_regex + " +"
		+ v_vt_vn_element_regex + "(?: +"
		+ v_vt_vn_element_regex + ")?$");

	std::regex regex_face_v_vt("^" + v_vt_element_regex + " +"
		+ v_vt_element_regex + " +"
		+ v_vt_element_regex + "(?: +"
		+ v_vt_element_regex + ")?$");

	std::regex regex_face_v_vn("^" + v_vn_element_regex + " +"
		+ v_vn_element_regex + " +"
		+ v_vn_element_regex + "(?: +"
		+ v_vn_element_regex + ")?$");

	std::regex regex_face_v("^" + index_regex + " +"
		+ index_regex + " +"
		+ index_regex + "(?: +"
		+ index_regex + ")?$");

	std::smatch m;
	for (std::string line; std::getline(file_in, line);) {
		// Vertices
		if (line.substr(0, 2) == "v ") {
			glm::vec4 vertex;
			std::string data = line.substr(2); // TODO: ask why I can't immediately pass this to regex match
			std::regex_match(data, m, regex_vertex_locations);

			if (m.empty()) {
				throw std::runtime_error(std::string("Could not parse: " + line));
			}

			vertex.x = std::stof(m[1]);
			vertex.y = std::stof(m[2]);
			vertex.z = std::stof(m[3]);

			if (m[4] != "") {
				vertex.w = std::stof(m[4]);
			}
			else {
				vertex.w = 1.0f;
			}

			vertices.push_back(vertex);
			vertex_normals.push_back(glm::vec3(0.0));
			vertex_normals_count.push_back(0);
		}
		// Texture Coordinates
		else if (line.substr(0, 3) == "vt ") {
			glm::vec3 uvw_coord;
			std::string data = line.substr(3); // TODO: ask why I can't immediately pass this to regex match
			std::regex_match(data, m, regex_texture_coordinates);

			if (m.empty()) {
				throw std::runtime_error(std::string("Could not parse: " + line));
			}

			uvw_coord.x = std::stof(m[1]);
			uvw_coord.y = std::stof(m[2]);

			if (m[3] != "") {
				uvw_coord.z = std::stof(m[3]);
			}
			else {
				uvw_coord.z = 0.0f;
			}

			uv_coords.push_back(uvw_coord);
		}
		// Face Normals
		else if (line.substr(0, 3) == "vn ") {
			glm::vec3 normal;
			std::string data = line.substr(3); // TODO: ask why I can't immediately pass this to regex match
			std::regex_match(data, m, regex_normals);

			if (m.empty()) {
				throw std::runtime_error(std::string("Could not parse: " + line));
			}

			normal.x = std::stof(m[1]);
			normal.y = std::stof(m[2]);
			normal.z = std::stof(m[3]);

			face_normals.push_back(normal);
		}
		// Parameter Space Vertices
		else if (line.substr(0, 3) == "vp ") {
			// we ignore this.
		}
		// Polygonal Face Elements
		else if (line.substr(0, 2) == "f ") {
			// TODO implement uv/normal linking, for now just ignore and obtain elements
			std::string data = line.substr(2); // TODO: ask why I can't immediately pass this to regex match

			glm::tvec3<glm::u32> triangle1_elements;
			glm::tvec3<glm::u32> triangle2_elements;
			bool is_quad = false;

			// TODO extract method of extracting elements and all, maybe nice build pattern?
			if (std::regex_match(data, m, regex_face_v_vt_vn)) {
				// data is in v_vt_vn 
				indicesToTriangleVector(m, 0, 3,
					triangle1_elements,
					triangle2_elements);

				faceNormalsToVertexNormals(m, 0, 2, 3,
					face_normals,
					vertex_normals,
					vertex_normals_count);


				bool is_quad = (m[10] != "");
			}
			else if (std::regex_match(data, m, regex_face_v_vn)) {
				// data is in v_vn
				indicesToTriangleVector(m, 0, 2,
					triangle1_elements,
					triangle2_elements);

				faceNormalsToVertexNormals(m, 0, 1, 2,
					face_normals,
					vertex_normals,
					vertex_normals_count);


				bool is_quad = (m[7] != "");
			}
			else if (std::regex_match(data, m, regex_face_v_vt)) {
				// data is in v_vt 
				indicesToTriangleVector(m, 0, 2,
					triangle1_elements,
					triangle2_elements);
				bool is_quad = (m[7] != "");
			}
			else if (std::regex_match(data, m, regex_face_v)) {
				// data is in v
				indicesToTriangleVector(m, 0, 1,
					triangle1_elements,
					triangle2_elements);
				bool is_quad = (m[4] != "");
			}
			else {
				// could not parse
				throw std::runtime_error(std::string("Could not parse: " + line));
			}

			elements.push_back(triangle1_elements);
			if (is_quad) {
				elements.push_back(triangle2_elements);
			}
		}
		// Comment line, always ignored
		else if (line.substr(0, 2) == "# ") {
		}
		// Garbage line.
		else {
			// we ignore this.
		}
	}

	// normalize vectors
	for (unsigned int i = 0; i < vertex_normals.size(); i++) {
		if (vertex_normals_count[i] != 0) {
			vertex_normals[i] /= (float)vertex_normals_count[i];
		}
	}

	// construct mesh
	int i = this->world.addMesh(Mesh(vertices,
		                             vertex_normals,
		                             uv_coords,
		                             elements));
	this->obj_mesh = this->world.getMeshPointer(i);
}

Object* ObjConstructor::add(std::string name, 
	                        nTiled_pipeline::ShaderId shader_id,
	                        glm::mat4 transformationMatrix) {
	int i = this->world.addObject(Object(name, 
		                          *(this->obj_mesh), 
		                          shader_id,
		                          transformationMatrix));
	return this->world.getObjectPointer(i);
}