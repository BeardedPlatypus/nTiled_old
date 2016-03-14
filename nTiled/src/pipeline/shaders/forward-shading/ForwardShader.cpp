#include "pipeline\shaders\forward-shading\ForwardShader.h"

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "pipeline\shader-util\LoadShaders.h"
#include "camera\Camera.h"

namespace nTiled {
namespace pipeline {

// ----------------------------------------------------------------------------
//  BasicForwardLightShader Base class
// ----------------------------------------------------------------------------
ForwardShader::ForwardShader(ForwardShaderId shader_id,
                             const std::string& path_vertex_shader,
                             const std::string& path_fragment_shader) :
  id(shader_id),
  objects(std::vector<PipelineObject>()),
  path_vertex_shader(path_vertex_shader),
  path_fragment_shader(path_fragment_shader) {
}

void ForwardShader::addObject(const world::Object& object) {
  // Setup initial Buffer Objects
  // --------------------------------------------------------------------------
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

void ForwardShader::addLight(const world::PointLight* light) {
  this->lights.push_back(light);

  PipelineLightData data = { light->position,
                   light->intensity,
                   light->cutoff_radius,
                   light->is_emitting };
  this->light_data.push_back(data);
}

void ForwardShader::init(const camera::Camera& camera) {
  // Create shader program
  // ------------------------------------------------------------------------
  this->loadShaders();

  // TODO: add function to change perspective matrix.
  // Setup perspective matrix
  // ------------------------------------------------------------------------
  glm::mat4 perspective_matrix = camera.getPerspectiveMatrix();
  GLint p_cameraToClip = glGetUniformLocation(this->shader_program,
                                              "cameraToClipMatrix");

  glUseProgram(this->shader_program);
  glUniformMatrix4fv(p_cameraToClip,
                     1, GL_FALSE,
                     glm::value_ptr(perspective_matrix));
  glUseProgram(0);

  // Setup unchangable light aspects
  // ------------------------------------------------------------------------
  // generate UBO
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
    GLint p_lightBlock = glGetUniformBlockIndex(this->shader_program,
                                                "LightBlock");
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->light_ubo);
    glUniformBlockBinding(this->shader_program,
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
}


// ----------------------------------------------------------------------------
//  Actual subclasses
// ----------------------------------------------------------------------------
ForwardVertLightingShader::ForwardVertLightingShader(
  ForwardShaderId shader_id,
  const std::string& path_vertex_shader,
  const std::string& path_fragment_shader) : ForwardShader(
    shader_id,
    path_vertex_shader,
    path_fragment_shader) {
}

void ForwardVertLightingShader::render(const camera::Camera& camera) {
  // Set shader program to this
  glUseProgram(this->shader_program);

  // Get the lookAt matrix
  glm::mat4 lookAt = camera.getLookAt();

  // Update light data
  glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
  for (GLuint i = 0; i < this->light_data.size(); i++) {
    glm::vec4 lightCameraCoordinates =
      lookAt * this->light_data[i].positionCoordinates;
    glBufferSubData(
      GL_UNIFORM_BUFFER,
      sizeof(this->light_data[0]) * i,
      sizeof(lightCameraCoordinates),
      glm::value_ptr(lightCameraCoordinates)
      );
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // render each model
  for (PipelineObject obj : this->objects) {
    glBindVertexArray(obj.vao);

    // retrieve memory locations shared per model
    GLint p_modelToCamera = glGetUniformLocation(
      this->shader_program,
      "modelToCameraMatrix");
    GLint p_normalModelToCamera = glGetUniformLocation(
      this->shader_program,
      "normalModelToCameraMatrix");

    // calculate transformation matrices per model
    glm::mat4 modelToCamera = lookAt * obj.transformation;

    glm::mat3 normalModelToCamera =
      glm::transpose(glm::inverse(glm::mat3(modelToCamera)));

    glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
                       glm::value_ptr(modelToCamera));
    glUniformMatrix3fv(p_normalModelToCamera, 1, GL_FALSE,
                       glm::value_ptr(normalModelToCamera));

    // render the elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 obj.element_buffer);
    glDrawElements(GL_TRIANGLES,
                   obj.element_buffer_size,
                   GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}

void ForwardVertLightingShader::loadShaders() {
  // Vertex Shader
  // -----------------------------------------------------------------
  // load file and preprocess
  std::stringstream vertexShaderBuffer;

  std::string replaceLine = "#define NUM_LIGHTS ";
  std::string n_lights = std::to_string(this->light_data.size());

  std::ifstream file_in(this->path_vertex_shader, std::ifstream::in);

  for (std::string line; std::getline(file_in, line);) {
    if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
      vertexShaderBuffer << replaceLine << n_lights << std::endl;
    } else {
      vertexShaderBuffer << line << std::endl;
    }
  }

  GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
                                      vertexShaderBuffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream fragmentShaderBuffer =
    readShader(this->path_fragment_shader);

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
                                        fragmentShaderBuffer.str());
  this->shader_program = createProgram(vertexShader, fragmentShader);
}

// ----------------------------------------------------------------------------
ForwardFragLightingShader::ForwardFragLightingShader(
  ForwardShaderId shader_id,
  const std::string& path_vertex_shader,
  const std::string& path_fragment_shader) : ForwardShader(
    shader_id,
    path_vertex_shader,
    path_fragment_shader) {
}

void ForwardFragLightingShader::render(const camera::Camera& camera) {
  // Set shader program to this
  glUseProgram(this->shader_program);
  // Get the lookAt matrix
  glm::mat4 lookAt = camera.getLookAt();

  // render each model
  for (PipelineObject obj : this->objects) {
    glBindVertexArray(obj.vao);
    // retrieve memory locations shared per model
    GLint p_modelToCamera = glGetUniformLocation(
      this->shader_program,
      "modelToCameraMatrix");

    // calculate transformation matrices per model
    glm::mat4 modelToCamera = lookAt * obj.transformation;
    glUniformMatrix4fv(p_modelToCamera, 1, GL_FALSE,
                       glm::value_ptr(modelToCamera));

    // Update light data
    glm::mat4 worldToModelSpace = glm::inverse(obj.transformation);

    glBindBuffer(GL_UNIFORM_BUFFER, this->light_ubo);
    for (GLuint i = 0; i < this->light_data.size(); i++) {
      glm::vec4 lightModelCoordinates =
        worldToModelSpace * this->light_data[i].positionCoordinates;
      glBufferSubData(
        GL_UNIFORM_BUFFER,
        sizeof(this->light_data[0]) * i,
        sizeof(lightModelCoordinates),
        glm::value_ptr(lightModelCoordinates)
        );
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // render the elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 obj.element_buffer);
    glDrawElements(GL_TRIANGLES,
                   obj.element_buffer_size,
                   GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}

void ForwardFragLightingShader::loadShaders() {
  // Vertex Shader
  // -----------------------------------------------------------------
  std::stringstream vertexShaderBuffer =
    readShader(this->path_vertex_shader);
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
                                      vertexShaderBuffer.str());

  // Fragment Shader
  // -----------------------------------------------------------------
  std::stringstream fragmentShaderBuffer;

  std::string replaceLine = "#define NUM_LIGHTS ";
  std::string n_lights = std::to_string(this->light_data.size());

  std::ifstream file_in(this->path_fragment_shader, std::ifstream::in);

  for (std::string line; std::getline(file_in, line);) {
    if (line.compare(0, replaceLine.size(), replaceLine) == 0) {
      fragmentShaderBuffer << replaceLine << n_lights << std::endl;
    } else {
      fragmentShaderBuffer << line << std::endl;
    }
  }

  std::cout << std::endl;
  std::cout << fragmentShaderBuffer.str() << std::endl;

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
                                        fragmentShaderBuffer.str());

  this->shader_program = createProgram(vertexShader, fragmentShader);
}

} // pipeline
} // nTiled