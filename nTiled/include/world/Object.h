#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm\glm.hpp>

#include <string>
#include <vector>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shaders\ShaderDefinitions.h"

namespace nTiled {
namespace world {

// ------------------------------------------------------------------------
//  World structs
// ------------------------------------------------------------------------
struct Mesh {
 public:
  Mesh(std::vector<glm::vec4> vertices,
       std::vector<glm::vec3> normals,
       std::vector<glm::vec3> uvs,
       std::vector<glm::tvec3<glm::u32>> elements);

  const std::vector<glm::vec4> vertices;
  const std::vector<glm::vec3> normals;
  const std::vector<glm::vec3> uvs;
  const std::vector<glm::tvec3<glm::u32>> elements;

 private:
  Mesh();
};

// ------------------------------------------------------------------------
struct Object {
public:
  Object(std::string name,
         Mesh mesh,
         pipeline::ShaderKey shader_id,
         glm::mat4 transformationMatrix);
  Object(std::string name,
         world::Mesh mesh,
         glm::mat4 transformationMatrix,
         pipeline::ShaderKey shader_id,
         bool is_rendered);


  const std::string name;
  const world::Mesh mesh;
  glm::mat4 transformationMatrix;

  pipeline::ShaderKey shader_key;

  bool is_rendered;
private:
  Object();
};

} // world
} // nTiled