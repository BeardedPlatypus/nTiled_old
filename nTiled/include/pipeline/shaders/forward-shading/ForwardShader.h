#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\PipelineObject.h"
#include "pipeline\light-management\Tiled\TiledLightManager.h"

namespace nTiled {
namespace pipeline {
// =========================================================================
//  ForwardLight Abstract
// =========================================================================
/*!
 ForwardShader is responsible for rendering any objects assigned to it,
 with the specified openGL shader code utilising a forward pipeline.
 */
class ForwardShader {
public:
  // --------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------
  /*!
   Construct a new ForwardShader

   Construct a new ForwardShader with the specified ForwardShaderId
   and vertex and fragment shader paths

   Args:
       shader_id (ForwardShaderId): the ForwardShaderId of this new
                                    ForwardShader
       path_vertex_shader (std::string): path to the vertex shader
                                         file.
       path_fragment_shader (std::string): path to the fragment shader
                                           file.

   Returns:
       A new ForwardShader of ForwardShaderId with the given shader
       files paths.
   */
  ForwardShader(ForwardShaderId shader_id,
                const std::string& path_vertex_shader,
                const std::string& path_fragment_shader);

  // --------------------------------------------------------------------
  //  Attribute Getters
  // --------------------------------------------------------------------
  /*!
   Get the ForwardShaderId of this ForwardShader

   Returns:
       The ForwardShaderId of this ForwardShader.
   */
  ForwardShaderId getId() const { return this->id; }

  // --------------------------------------------------------------------
  //  Interaction functions
  // --------------------------------------------------------------------
  /*!
   Initialise this ForwardShader

   Initialise this ForwardShader by compiling the GLSL code and setting
   up static openGL attributes and memory.

   Args:
       camera (Camera&): The camera used to render with this
                       ForwardShader.
   */
  virtual void init(const camera::Camera& camera);

  /*!
   Render all objects of this ForwardShader

   Render all objects of this ForwardShader with the compiled shaders.

   Args:
       camera (Camera&): The camera used to render with this
                       ForwardShader
   */
  virtual void render(const camera::Camera& camera) = 0;

  // --------------------------------------------------------------------
  //  Object Management functions
  // --------------------------------------------------------------------
  /*!
   Add the specified nTiled_world::Object to this ForwardShader

   Args:
       obj (nTiled_world::Object&): The nTiled_world::Object to be added
                                  to this ForwardShader.
   */
  void addObject(const world::Object& obj);

  /*!
   Add the specified nTiled_world::PointLight to this ForwardShader

   Args:
      p_light (nTiled_world::PointLight*): pointer to the
                                         nTiled_world::PointLight to be
                       added to this ForwardShader
   */
  void addLight(const world::PointLight* p_light);

  /*!
   Load the ShaderFiles specified in with the paths during construction
   */
  virtual void loadShaders() = 0;
 protected:
  /*! ForwardShaderId of this ForwardShader */
  const ForwardShaderId id;

  /*! Pointers to the lights affecting this ForwardShader */
  std::vector<const world::PointLight*> lights;
  /*! Internal LightData of the lights affecting this ForwardShader */
  std::vector<PipelineLightData> light_data;

  /*! Internal ObjectData of the Objects added to this ForwardShader */
  std::vector<PipelineObject> objects;

  /*! GLuint pointer to the ShaderProgram constructed with LoadShaders */
  GLuint shader_program;
  /*!
   GLuint pointer to the Uniform Buffer Object holding the internal
   light data.
   */
  GLuint light_ubo;

  /*! Path to the vertex shader file of this ForwardShader.*/
  const std::string path_vertex_shader;
  /*! Path to the fragment shader file of this ForwardShader*/
  const std::string path_fragment_shader;
};

// =========================================================================
//  Solid Color Shader
// =========================================================================
class SolidShader : public ForwardShader {
 public:
  SolidShader(ForwardShaderId shader_id,
              const std::string& path_vertex_shader,
              const std::string& path_fragment_shader);

  void init(const camera::Camera& camera);
  void render(const camera::Camera& camera);
  void loadShaders();
};

// =========================================================================
//  Forward rendering: Vertex Lighting 
// =========================================================================
class ForwardVertLightingShader : public ForwardShader {
 public:
  ForwardVertLightingShader(ForwardShaderId shader_id,
                            const std::string& path_vertex_shader,
                            const std::string& path_fragment_shader);
  void render(const camera::Camera& camera);
  void loadShaders();
};

// =========================================================================
// Forward rendering: Fragment Lighting
// =========================================================================
class ForwardFragLightingShader : public ForwardShader {
 public:
  ForwardFragLightingShader(ForwardShaderId shader_id,
                            const std::string& path_vertex_shader,
                            const std::string& path_fragment_shader);
  void render(const camera::Camera& camera);
  void loadShaders();
};

// =========================================================================
// Forward rendering: Tiled Lighting
// =========================================================================
class ForwardTiledShader : public ForwardShader {
 public:
  ForwardTiledShader(ForwardShaderId shader_id,
                     const std::string& path_vertex_shader,
                     const std::string& path_fragment_shader,
                     glm::uvec2 tile_size,
                     const world::World& world,
                     const state::View& view);

  void init(const camera::Camera& camera);
  void render(const camera::Camera& camera);
  void loadShaders();
 private:
  /*! Sphere projector of this ForwardTiledShader */
  BoxProjector projector;
  /*! TiledLightManager of this ForwardTiledShader */
  TiledLightManager light_manager;

  /*!
   GLuint pointer to the Shader Storage Buffer Object storing
   the light grid of this ForwardTiledShader
   */
  GLuint light_grid_buffer;
  /*!
   GLuint pointer to the Shader Storage Buffer Object storing
   the light index of this ForwardTiledShader
   */
  GLuint light_index_buffer;
};

} // pipeline
} // nTiled
