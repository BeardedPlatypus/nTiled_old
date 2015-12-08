#pragma once

#include <glad\glad.h>
#include <string>

// ----------------------------------------------------------------------------
//  Shader loading and compiling.
// ----------------------------------------------------------------------------
GLuint loadAndCompileShader(GLenum shaderType, const std::string& path);
GLuint createVertexFragmentProgram(const std::string& vertex_shader_path,
	                               const std::string& fragment_shader_path);
