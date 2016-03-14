#pragma once

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\PipelineObject.h"
#include "pipeline\shaders\deferred-shading\GBuffer.h"

#include "pipeline\light-management\Tiled\TiledLightManager.h"

namespace nTiled {
namespace pipeline {
// =========================================================================
//  Deferred Light Abstract
// =========================================================================
/*!
 DeferredShader is responsible for rendering any objects assigned to it,
 with the specified openGL shader code utilising a deferred pipeline.
 */
class DeferredShader {
 public:
  // --------------------------------------------------------------------
  //  Constructor
  // --------------------------------------------------------------------
  /*!
   Construct a new DeferredShader

   Construct a new DeferredShader with the specified a DeferredShaderId
   and vertex and fragment shader paths to the geometry and light pass
   shaders.

   Args:
       shader_id (DeferredShaderId): the DeferredShaderId of this new
                                     DeferredShader
       path_geometry_pass_vertex_shader (std::string): path to the
         geometry pass vertex shader file.
       path_geometry_pass_fragment_shader (std::string): path to the
         geometry pass fragment shader file.
       path_light_pass_vertex_shader (std::string): path to the
         light pass vertex shader file.
       path_geometry_pass_fragment_shader (std::string): path to the
         light pass fragment shader file.
     state (nTiled_state::State&): Reference to the state this
                                   Deferred state belongs to.

   Returns:
       A new ForwardShader of ForwardShaderId with the given shader
       files paths.
   */
  DeferredShader(DeferredShaderId shader_id,
                 const std::string& path_geometry_pass_vertex_shader,
                 const std::string& path_geometry_pass_fragment_shader,
                 const std::string& path_light_pass_vertex_shader,
                 const std::string& path_light_pass_fragment_shader,
                 const world::World& world,
                 const state::View& view);

  // --------------------------------------------------------------------
  //  Attribute Getters
  // --------------------------------------------------------------------
  /*!
   Get the DeferredShaderId of this DeferredShader

   Returns:
       The DeferredShaderId of this DeferredShader.
   */
  DeferredShaderId getId() const { return this->id; }

  // --------------------------------------------------------------------
  //  Interaction functions
  // --------------------------------------------------------------------
  /*!
   Initialise this DeferredShader

   Initialise this DeferredShader by compiling the GLSL code and setting
   up static openGL attributes and memory.
   */
  virtual void init();

  /*!
   Render all objects of this DeferredShader

   Render all objects of this DeferredShader with the compiled shaders.
   */
  virtual void render();
  // --------------------------------------------------------------------
  /*!
   Render the geometry (first) pass of this DeferredShader

   Render all the attributes needed for the light (second) pass
   of this DeferredShader into the GBuffer of this DeferredShader.
   */
  virtual void renderGeometryPass();

  /*!
   Render the light (second) pass of this DeferredShader

   Render a quad and use the GBuffer to obtain the attributes of the
   scene to render the frame.
   */
  virtual void renderLightPass();

  /*!
   Render the different GBuffers to frame.
   */
  virtual void renderBuffers();

  // --------------------------------------------------------------------
  //  Object Management functions
  // --------------------------------------------------------------------
  /*!
   Add the specified nTiled_world::Object to this DeferredShader

   Args:
       obj (nTiled_world::Object&): The nTiled_world::Object to be added
                                  to this DeferredShader.
   */
  void addObject(const world::Object& obj);

  /*!
   Add the specified nTiled_world::PointLight to this DeferredShader

   Args:
      p_light (nTiled_world::PointLight*): pointer to the
                                         nTiled_world::PointLight to be
                       added to this DeferredShader
   */
  void addLight(const world::PointLight* p_light);

  /*!
   Load the ShaderFiles specified in with the paths during construction
   */
  void loadShaders();

protected:
  /*! DeferredShaderId of this DeferredShader*/
  const DeferredShaderId id;

  /*! World this DeferredShader observes */
  const world::World& world;
  /*! View this DeferredShader observes */
  const state::View& view;

  /*! Pointers to the lights affecting this ForwardShader */
  std::vector<const world::PointLight*> lights;
  /*! Internal LightData of the lights affecting this ForwardShader */
  std::vector<PipelineLightData> light_data;

  /*! Internal ObjectData of the Objects added to this ForwardShader */
  std::vector<PipelineObject> objects;

  /*! GLuint pointer to the geometry pass ShaderProgam */
  GLuint geometry_pass_sp;
  /*! GLuint pointer to the light pass ShaderProgram */
  GLuint light_pass_sp;

  /*!
   GLuint pointer to the Uniform Buffer Object holding the internal
   light data.
   */
  GLuint light_ubo;

  /*! GBuffer of this DeferredShader */
  GBuffer gBuffer;

  /*! Path to the vertex shader file of the Geometry Pass */
  const std::string path_geometry_pass_vertex_shader;
  /*! Path to the fragment shader file of the Geometry Pass*/
  const std::string path_geometry_pass_fragment_shader;

  /*! Path to the vertex shader file of the Light Pass */
  const std::string path_light_pass_vertex_shader;
  /*! Path to the fragment shader file of the Light Pass */
  const std::string path_light_pass_fragment_shader;

  /*! PipelineObject of the fullscreen quad used in the Light Pass */
  PipelineObject fullscreen_quad;
};


// ------------------------------------------------------------------------
//  Tiled Deferred Shading
// ------------------------------------------------------------------------
class TiledDeferredLightShader : public DeferredShader {
public:
  // constructor
  TiledDeferredLightShader(
      DeferredShaderId shader_id,
      const std::string& path_geometry_pass_vertex_shader,
      const std::string& path_geometry_pass_fragment_shader,
      const std::string& path_light_pass_vertex_shader,
      const std::string& path_light_pass_fragment_shader,
      const glm::uvec2& tile_size,
      const world::World& world,
      const state::View& view
      );

  // render functions
  void init();
  void renderLightPass();

private:
  /*! Sphere projector of this ForwardTiledShader */
  const BoxProjector projector;
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
