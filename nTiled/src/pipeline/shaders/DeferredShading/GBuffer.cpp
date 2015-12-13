#include "pipeline\shaders\DeferredShading\GBuffer.h"

#include <stdio.h>
#include <iostream>
#include <string>

using namespace nTiled_pipeline;

// ----------------------------------------------------------------------------
//  Constructor
// ----------------------------------------------------------------------------
GBuffer::GBuffer(unsigned int width, unsigned int height) :
	width(width), height(height) {}

// ----------------------------------------------------------------------------
//  Render functions
// ----------------------------------------------------------------------------
void GBuffer::init() {
	// Create FBO
	// ------------------------------------------------------------------------
	glGenFramebuffers(1, &(this->p_fbo));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, p_fbo);

	// Create the gbuffer textures
	// ------------------------------------------------------------------------
	glGenTextures(this->GBUFFER_NUM_TEXTURES, this->p_textures);
	glGenTextures(1, &(this->p_depth_texture));

	// Setup specific GBuffer textures
	// ------------------------------------------------------------------------
	for (unsigned int i = 0; i < this->GBUFFER_NUM_TEXTURES; i++) {
		// Bind texture
		glBindTexture(GL_TEXTURE_2D, this->p_textures[i]);
		// Specify texture
		glTexImage2D(GL_TEXTURE_2D,               // target
			         0,                           // level (mipmap level)
			         GL_RGB32F,                   // internal format
			         this->width, this->height,   // dimensions
			         0,                           // border
			         GL_RGB,                      // format
			         GL_UNSIGNED_SHORT_5_6_5,     // type
			         NULL                         // data
			);            
		// Attach texture the framebuffer object
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,       // target
			                   GL_COLOR_ATTACHMENT0 + i,  // attachment
			                   GL_TEXTURE_2D,             // textarget
			                   this->p_textures[i],       // texture
			                   0                          // level (mipmap level)
			);
	}

	// Setup Depth buffer
	// ------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, this->p_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 
		         0, 
		         GL_DEPTH_COMPONENT32F, 
		         this->width, this->height, 
		         0, 
		         GL_DEPTH_COMPONENT, 
		         GL_FLOAT,
		         NULL
		);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, 
		                   GL_DEPTH_ATTACHMENT, 
		                   GL_TEXTURE_2D, 
		                   this->p_depth_texture, 
		                   0
		);

	// Draw buffers
	// ------------------------------------------------------------------------
	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, 
		                      GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(this->GBUFFER_NUM_TEXTURES, draw_buffers);

	// Check if frame buffers have been properly created.
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error(std::string("FB error, status: 0x%x\n", status));
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}


// ----------------------------------------------------------------------------
//  Read / Write operation
// ----------------------------------------------------------------------------
void GBuffer::bindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);
}

void GBuffer::unbindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GBuffer::bindForReading() {
	glBindFramebuffer(GL_READ_BUFFER, this->p_fbo);
}

void GBuffer::setReadBuffer(GBUFFER_TEXTURE_TYPE texture_type) {
	glNamedFramebufferReadBuffer(this->p_fbo,
		                         GL_COLOR_ATTACHMENT0 + texture_type);
}

GLuint GBuffer::getPointerFBO() {
	return this->p_fbo;
}