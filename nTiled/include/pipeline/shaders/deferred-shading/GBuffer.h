#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <glad\glad.h>

namespace nTiled {
namespace pipeline {

class GBuffer {
 public:
  enum GBUFFER_TEXTURE_TYPE {
    GBUFFER_TEXTURE_TYPE_DIFFUSE,
    GBUFFER_TEXTURE_TYPE_NORMAL,
    GBUFFER_NUM_TEXTURES
  };

  // Constructor | Destructor
  GBuffer(unsigned int width, unsigned int height);

  // Render related functions
  void init();

  void bindForWriting();
  void unbindForWriting();

  void bindForReading();
  void setReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);

  GLuint getPointerFBO();

private:
  unsigned int width;
  unsigned int height;

  GLuint p_fbo;
  GLuint p_textures[GBUFFER_NUM_TEXTURES];
  GLuint p_depth_texture;
};

} // pipeline
} // nTiled