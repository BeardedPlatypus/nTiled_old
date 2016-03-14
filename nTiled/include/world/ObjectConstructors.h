#pragma once


// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "world\World.h"

namespace nTiled {
namespace world {
// ------------------------------------------------------------------------
//  Abstract class
class PrimitiveConstructor {
 public:
  virtual ~PrimitiveConstructor() {}
  virtual Object* add(const std::string& name,
                      pipeline::ShaderKey shader_id,
                      glm::mat4 transformationMatrix) = 0;
};

// ------------------------------------------------------------------------
//  Cube meshes
class CubeConstructor : public PrimitiveConstructor {
 public:
  CubeConstructor(World& world);
  Object* add(const std::string& name,
              pipeline::ShaderKey shader_id,
              glm::mat4 transformationMatrix);

 private:
  World& world;
  const Mesh* cube;
};

// ------------------------------------------------------------------------
//  Obj Mesh loader
class ObjConstructor : public PrimitiveConstructor {
 public:
  ObjConstructor(const std::string& path,
                 World& world);

  Object* add(const std::string& name,
              pipeline::ShaderKey shader_id,
              glm::mat4 transformationMatrix);
 private:
  World& world;
  const Mesh* obj_mesh;
};


class AssImpConstructor : public PrimitiveConstructor {
 public:
  AssImpConstructor(); // ugly but tuple requires empty constructor does nothing, do not use
  AssImpConstructor(const std::string& path,
                    World& world);

  Object* add(const std::string& name,
              pipeline::ShaderKey shader_id,
              glm::mat4 transformationMatrix);
 private:
  World& world;
  const Mesh* obj_mesh;
};


// ------------------------------------------------------------------------
//  Icosphere loader
class IcosphereConstructor : public PrimitiveConstructor {
public:
  IcosphereConstructor(World& world);
  Object* add(const std::string& name,
              pipeline::ShaderKey shader_id,
              glm::mat4 transformationMatrix);
private:
  World& world;
  const Mesh* icosphere;
};

} // world
} // nTiled