#include "pipeline\shaders\deferred-shading\DeferredShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

#include <algorithm>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"

namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  BasicDeferredLightShader class
// ----------------------------------------------------------------------------
//  Constructor
DeferredShader::DeferredShader(
  DeferredShaderId shader_id,
  const std::string& path_geometry_pass_vertex_shader,
  const std::string& path_geometry_pass_fragment_shader,
  const std::string& path_light_pass_vertex_shader,
  const std::string& path_light_pass_fragment_shader,
  const world::World& world,
  const state::View& view)
  : id(shader_id),
  objects(std::vector<PipelineObject>()),
  path_geometry_pass_vertex_shader(path_geometry_pass_vertex_shader),
  path_geometry_pass_fragment_shader(path_geometry_pass_fragment_shader),
  path_light_pass_vertex_shader(path_light_pass_vertex_shader),
  path_light_pass_fragment_shader(path_light_pass_fragment_shader),
  world(world),
  view(view),
  gBuffer(GBuffer(view.viewport.x, view.viewport.y)) {
}

// ----------------------------------------------------------------------------
//  Property interaction methods
void DeferredShader::addObject(const world::Object& object) {
  // Setup initial Buffer Objects
  // ------------------------------------------------------------------------
  // Set up position buffer of the vertices
  GLuint vbo_handles[3]; // create 3 vertex buffer objects
  glGenBuffers(3, vbo_handles);

  GLuint position_buffer = vbo_handles[0];
  GLuint normal_buffer = vbo_handles[1];
  GLuint element_buffer = vbo_handles[2];
  GLuint vao;

  // Set up vertex array object and enable vertex array zero and one
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // TODO: this might get changed as world gets implemented
  // Set up geometric data.
  // ------------------------------------------------------------------------
  // set up position buffer
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer); // set position buffer to GL_ARRAY_BUFFEr
                          // not part of the VAO yet.
  glBufferData(GL_ARRAY_BUFFER,
               object.mesh.vertices.size() * sizeof(glm::vec4),
               &object.mesh.vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0,         // attribute
                        4,         // size 
                        GL_FLOAT,  // type 
                        GL_FALSE,  // normalized? 
                        0,         // stride 
                        NULL       // array buffer offset
                        );

  // set up normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               object.mesh.normals.size() * sizeof(glm::vec3),
               &object.mesh.normals[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  // set up element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               object.mesh.elements.size() * sizeof(glm::tvec3<glm::u32>),
               &object.mesh.elements[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, GL_FALSE, 0, NULL);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  this->objects.push_back(PipelineObject(vao,
                                         element_buffer,
                                         object.mesh.elements.size() * 3,
                                         object.transformationMatrix));
  glBindVertexArray(0);
}

void DeferredShader::addLight(const world::PointLight* light) {
  this->lights.push_back(light);

  PipelineLightData data = { light->position,
                           light->intensity,
                           light->cutoff_radius,
                           light->is_emitting };
  this->light_data.push_back(data);
}

// ----------------------------------------------------------------------------
//  Render related methods
void DeferredShader::init() {
  // Create shader program
  // ------------------------------------------------------------------------
  this->loadShaders();

  // Create Frame Buffer Objects (FBO)
  this->gBuffer.init();


  // Setup for the geometry pass.
  // ------------------------------------------------------------------------
  // Setup perspective matrix
  glm::mat4 perspective_matrix = this->view.camera.getPerspectiveMatrix();
  glm::mat4 inverse_perspective_matrix = glm::inverse(perspective_matrix);

  GLint p_cameraToClip =
    glGetUniformLocation(this->geometry_pass_sp,
                         "cameraToClipMatrix");

  glUseProgram(this->geometry_pass_sp);
  glUniformMatrix4fv(p_cameraToClip,
                     1, GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUseProgram(0);

  // Setup for the light pass
  // ------------------------------------------------------------------------
  // Setup perspective matrices, viewport, depthrange
  GLint p_perspectiveMatrix = glGetUniformLocation(this->light_pass_sp,
                                                   "perspectiveMatrix");
  GLint p_invPerspectiveMatrix = glGetUniformLocation(this->light_pass_sp,
                                                      "invPerspectiveMatrix");

  GLint p_viewport = glGetUniformLocation(this->light_pass_sp,
                                          "viewport");
  GLint p_depthrange = glGetUniformLocation(this->light_pass_sp,
                                            "depthrange");

  glm::vec4 viewport = glm::vec4(0.0f, 0.0f, this->view.viewport.x,
                                 this->view.viewport.y);
  glm::vec2 depthrange = this->view.camera.getDepthrange();

  glUseProgram(this->light_pass_sp);
  glUniformMatrix4fv(p_perspectiveMatrix,
                     1, GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUniformMatrix4fv(p_invPerspectiveMatrix,
                     1, GL_FALSE,
                     glm::value_ptr(inverse_perspective_matrix));

  glUniform4fv(p_viewport, 1, glm::value_ptr(viewport));
  glUniform2fv(p_depthrange, 1, glm::value_ptr(depthrange));
  glUseProgram(0);

  // Setup unchangable light aspects
  if (this->light_data.size() > 0) {
    GLuint num_lights = this->light_data.size();

    glGenBuffers(1, &this->light_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER,
                 this->light_ubo);

    glBufferData(GL_UNIFORM_BUFFER,
                 sizeof((this->light_data[0])) * num_lights,
                 this->light_data.data(),
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // link UBO with the shader
    GLint p_lightBlock = glGetUniformBlockIndex(this->light_pass_sp,
                                                "LightBlock");
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
    glUniformBlockBinding(this->light_pass_sp,
                          p_lightBlock,
                          0);

    // add initial data
    glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p,
           this->light_data.data(),
           sizeof(this->light_data[0]) * num_lights);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  // associate textures with sampler objects in glsl
  GLint p_diffuseTexture = glGetUniformLocation(this->light_pass_sp,
                                                "diffuseTexture");
  GLint p_normalTexture = glGetUniformLocation(this->light_pass_sp,
                                               "normalTexture");
  GLint p_depthTexture = glGetUniformLocation(this->light_pass_sp,
                                              "depthTexture");

  glUseProgram(this->light_pass_sp);
  glUniform1i(p_diffuseTexture,
              GL_TEXTURE0);// +this->gBuffer.GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glUniform1i(p_normalTexture,
              GL_TEXTURE1);//GL_TEXTURE0 + this->gBuffer.GBUFFER_TEXTURE_TYPE_NORMAL);
  glUniform1i(p_depthTexture,
              GL_TEXTURE2);//GL_TEXTURE0 + this->gBuffer.GBUFFER_NUM_TEXTURES);

  glUseProgram(0);


  // Create fullscreen quad
  // ---------------------------------------------------------------------------
  // Position data
  GLfloat quad_vertices[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
  };

  // Element data
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

  this->fullscreen_quad = PipelineObject(vao,
                                         element_buffer,
                                         6,
                                         glm::mat4(1.0f));
}

void DeferredShader::render() {
  this->renderGeometryPass();
  this->renderLightPass();
  //this->renderBuffers();
}

void DeferredShader::renderGeometryPass() {
  // Setup GBuffer
  // ---------------------------------------------------------------------------
  // enable writing to gBuffer
  this->gBuffer.bindForWriting();

  // Only the geometry pass updates the depth buffer
  //glDepthMask(GL_TRUE);

  // clear old contents gBuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glEnable(GL_DEPTH_TEST);
  //glDisable(GL_BLEND);

  // Rendering to GBuffer
  // ---------------------------------------------------------------------------
  // Set shader program to geometry pass
  glUseProgram(this->geometry_pass_sp);

  glm::mat4 lookAt = this->view.camera.getLookAt();
  for (PipelineObject obj : this->objects) {
    glBindVertexArray(obj.vao);

    // retrieve memory locations shared per model
    GLint p_modelToCamera = glGetUniformLocation(this->geometry_pass_sp,
                                                 "modelToCameraMatrix");
    glm::mat4 modelToCamera = lookAt * obj.transformation;
    glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
                       glm::value_ptr(modelToCamera));

    GLint p_normalModelToCamera = glGetUniformLocation(this->geometry_pass_sp,
                                                       "invTransModelToCameraMatrix");
    glm::mat4 invTransModelToCamera = glm::inverseTranspose(modelToCamera);
    glUniformMatrix4fv(p_normalModelToCamera, 1, GL_FALSE,
                       glm::value_ptr(invTransModelToCamera));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 obj.element_buffer);
    glDrawElements(GL_TRIANGLES,
                   obj.element_buffer_size,
                   GL_UNSIGNED_INT, 0);
  }

  this->gBuffer.unbindForWriting();
  glUseProgram(0);

  //glDepthMask(GL_FALSE);
  //glDisable(GL_DEPTH_TEST);
}


void DeferredShader::renderLightPass() {
  // use light pass shader
  glUseProgram(this->light_pass_sp);

  // bind the texture for reading
  this->gBuffer.bindForReading();

  // Update light positions
  glm::mat4 lookAt = this->view.camera.getLookAt();
  glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
  for (GLuint i = 0; i < this->light_data.size(); i++) {
    glm::vec4 lightCameraCoordinates =
      lookAt * this->light_data[i].positionCoordinates;

    glBufferSubData(GL_UNIFORM_BUFFER,
                    sizeof(this->light_data[0]) * i,
                    sizeof(lightCameraCoordinates),
                    glm::value_ptr(lightCameraCoordinates));
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  // Render elements
  glBindVertexArray(this->fullscreen_quad.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
               this->fullscreen_quad.element_buffer);
  glDrawElements(GL_TRIANGLES,
                 this->fullscreen_quad.element_buffer_size,
                 GL_UNSIGNED_SHORT, 0);
  glUseProgram(0);
}


void DeferredShader::renderBuffers() {
  // TODO
  // Restore default frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setup GBuffer
  //this->gBuffer.bindForReading();

  // FIXME eeeks hard coded constants
  GLuint width = this->view.viewport.x;
  GLuint height = this->view.viewport.y;
  GLsizei HalfWidth = (GLsizei)(width / 2.0f);
  GLsizei HalfHeight = (GLsizei)(height / 2.0f);

  /*
  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitNamedFramebuffer(this->gBuffer.getPointerFBO(), 0,
                       0, 0, WIDTH, HEIGHT,
                       0, 0, WIDTH, HEIGHT,
                       GL_COLOR_BUFFER_BIT, GL_LINEAR);
  */

  /*
  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitNamedFramebuffer(this->gBuffer.getPointerFBO(), 0,
    0, 0, WIDTH, HEIGHT,
    0, 0, WIDTH, HEIGHT,
    GL_COLOR_BUFFER_BIT, GL_LINEAR);
  */

  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
  glBlitNamedFramebuffer(this->gBuffer.getPointerFBO(), 0,
                         0, 0, width, height,
                         0, HalfHeight, HalfWidth, height,
                         GL_COLOR_BUFFER_BIT, GL_LINEAR);

  this->gBuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
  glBlitNamedFramebuffer(this->gBuffer.getPointerFBO(), 0,
                         0, 0, width, height,
                         HalfWidth, HalfHeight, width, height,
                         GL_COLOR_BUFFER_BIT, GL_LINEAR);


}

void DeferredShader::loadShaders() {
  // Geometry Pass
  std::stringstream geometryPassVertexShaderBuffer =
    readShader(this->path_geometry_pass_vertex_shader);
  GLuint geometryPassVertexShader = compileShader(GL_VERTEX_SHADER,
                                                  geometryPassVertexShaderBuffer.str());

  std::stringstream geometryPassFragmentShaderBuffer =
    readShader(this->path_geometry_pass_fragment_shader);
  GLuint geometryPassFragmentShader = compileShader(GL_FRAGMENT_SHADER,
                                                    geometryPassFragmentShaderBuffer.str());

  this->geometry_pass_sp = createProgram(geometryPassVertexShader,
                                         geometryPassFragmentShader);

  // Light Pass
  std::stringstream lightPassVertexShaderBuffer =
    readShader(this->path_light_pass_vertex_shader);
  GLuint lightPassVertexShader = compileShader(GL_VERTEX_SHADER,
                                               lightPassVertexShaderBuffer.str());

  std::stringstream lightPassFragmentShaderBuffer;
  std::string replaceLine = "#define NUM_LIGHTS ";
  std::string n_lights = std::to_string(std::max((int) this->light_data.size(), 1));

  std::ifstream file_in(this->path_light_pass_fragment_shader,
                        std::ifstream::in);

  for (std::string line; std::getline(file_in, line);) {
    if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
      lightPassFragmentShaderBuffer << replaceLine << n_lights << std::endl;
    } else {
      lightPassFragmentShaderBuffer << line << std::endl;
    }
  }

  GLuint lightPassFragmentShader = compileShader(GL_FRAGMENT_SHADER,
                                                 lightPassFragmentShaderBuffer.str());

  this->light_pass_sp = createProgram(lightPassVertexShader,
                                      lightPassFragmentShader);
}

} // pipeline
} // nTiled