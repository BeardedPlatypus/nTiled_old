
#include "pipeline\Pipeline.h"

#include <glm/gtc/type_ptr.hpp>
#include "pipeline\PipelineObject.h"
#include "pipeline\shader-util\LoadShaders.h"

#include "pipeline\shaders\ShaderFactory.h"

#include <set>
#include <iostream>
#include <fstream>
#include <sstream>


#define VERT_SHADER_PATH std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/pass/pass.vert")
#define FRAG_SHADER_PATH std::string("./src/pipeline/shaders-glsl/forward-shading/debug-shaders/pass/pass.frag")



using namespace nTiled_pipeline;

// ---------------------------------------------------------------------------
//  Forward Pipeline
// ---------------------------------------------------------------------------
//  Constructor
ForwardDebugPipeline::ForwardDebugPipeline(nTiled_state::State& state) :
	Pipeline(state),
	objects(std::vector<PipelineObject>()),
	shaders(std::map<ShaderId, ShaderBatch*>()),
	quad_writer(ConstantQuadWriter()),
	draw_pretty_squares(LightProjectorQuadWriter(state)),
	projector(BoxProjector()),
	manager(TiledLightManager(state, 1200, 1200, this->projector)),
	tiles_display(LightTilesDisplay(this->manager)) {
	std::cout << "forward pipeline" << std::endl;
	std::cout << "number of objects: " << state.world.objects.size() << std::endl;
	
	// setup objects and add all relevant shaders
	// ------------------------------------------------------------------------
	for (nTiled_world::Object object : this->state.world.objects) {
		std::cout << "object" << std::endl;
		this->addObject(object);
	}

	// Add lights to all relevant shaders 
	// ------------------------------------------------------------------------
	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		for (nTiled_world::PointLight& pointLight : this->state.world.lights) {
			b->addLight(&pointLight);
		}
	}

	// initialise each shader
	// ------------------------------------------------------------------------
	ShaderBatch* b_p;
	for (const auto& shader_couple : this->shaders) {
		b_p = shader_couple.second;
		b_p->init(this->state.camera);
	}

	// initialise framebuffer object
	// ------------------------------------------------------------------------
	// Create FBO
	glGenFramebuffers(1, &(this->p_fbo));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);

	// Create Texture pass result
	glGenTextures(1, &(this->p_pass_result)); 

	// Set up created texture
	glBindTexture(GL_TEXTURE_2D, this->p_pass_result);
	glTexImage2D(GL_TEXTURE_2D,               // target
			     0,                           // level (mipmap level)
			     GL_RGB32F,                   // internal format
			     this->state.viewport.x,
		         this->state.viewport.y,   // dimensions
			     0,                           // border
			     GL_RGB,                      // format
			     GL_UNSIGNED_SHORT_5_6_5,     // type
			     NULL                         // data
		);       

	// set 1 to 1 mapping so no interpolation between pixels occurs
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attach texture the framebuffer object
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,       // target
			               GL_COLOR_ATTACHMENT0,      // attachment
			               GL_TEXTURE_2D,             // textarget
			               this->p_pass_result,       // texture
			               0                          // level (mipmap level)
		);

	// draw bufers
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error(std::string("FB error, status: 0x%x\n", status));
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Create fullscreen quad
	// ---------------------------------------------------------------------------
	// Position data
	GLfloat quad_vertices[] = {
		-1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, 1.0f
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

	// Create render program
	// -----------------------------------------------------------------------

	// Vertex Shader
	// -----------------------------------------------------------------
	std::stringstream vertexShaderBuffer =
		readShader(VERT_SHADER_PATH);
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER,
		vertexShaderBuffer.str());

	// Fragment Shader
	// -----------------------------------------------------------------
	std::stringstream fragmentShaderBuffer = 
		readShader(FRAG_SHADER_PATH);

	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER,
		                                  fragmentShaderBuffer.str());
	this->result_pass_sp = createProgram(vertexShader, fragmentShader);


	GLint p_viewport = glGetUniformLocation(this->result_pass_sp,
		                                    "viewport");

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 
		                           this->state.viewport.x, 
		                           this->state.viewport.y);


	GLint p_resultTexture = glGetUniformLocation(this->result_pass_sp,
		                                          "resultTexture");
	glUseProgram(this->result_pass_sp);
	glUniform4fv(p_viewport, 1, glm::value_ptr(viewport));
	glUniform1i(p_resultTexture,
		        GL_TEXTURE0);
	glUseProgram(0);

	// ------------------------------------------------------------------------
	//this->quad_writer.addQuad(glm::vec4(0.0, 0.0, 1.0, 1.0));
}

// ----------------------------------------------------------------------------
//  Render functions
void ForwardDebugPipeline::render() {
	// activate frame buffer object for writing
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->p_fbo);

	// clear old contents
	glClear(GL_COLOR_BUFFER_BIT);

	// FIXME make this nice and incorporate with gui
	//std::set<ShaderId> disabled_shaders = {};
	std::set<ShaderId> disabled_shaders = { ShaderId::ForwardDebugCoreLight,
				  						    ShaderId::ForwardDebugCutOffLight
	                                        };

	for (const auto& shader_couple : this->shaders) {
		ShaderBatch* b = shader_couple.second;
		if (disabled_shaders.count(b->getId()) == 0) {
			b->render(this->state.camera);
		}
	}

	// deactivvate frame buffer object for writing
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// activate texture for render shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->p_pass_result);

	glUseProgram(this->result_pass_sp);

	glBindVertexArray(this->fullscreen_quad.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		         this->fullscreen_quad.element_buffer);
	glDrawElements(GL_TRIANGLES,
		           this->fullscreen_quad.element_buffer_size,
		           GL_UNSIGNED_SHORT, 0);
	glUseProgram(0);
	//this->quad_writer.render();

	this->manager.constructGridFrame();
	this->tiles_display.render();
	this->draw_pretty_squares.render();
}

// ----------------------------------------------------------------------------
//  Object loading
void ForwardDebugPipeline::addObject(nTiled_world::Object& object) {
	// Add shader if it has not been initialised yet.
	// ------------------------------------------------------------------------
	if (this->shaders.count(object.shader_id) == 0) {
		// FIXME Rework this together with shaderfactor
		ShaderFactory shader_factory = ShaderFactory(this->state);
		this->shaders.insert(std::pair<ShaderId, ShaderBatch*>(
			object.shader_id,
			shader_factory.getShader(object.shader_id, this->state)));
	}

	// Add object to shader
	// -------------------------------------------------------------------------
	this->shaders[object.shader_id]->addObject(object);
}
