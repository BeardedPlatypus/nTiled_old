#pragma once

// ----------------------------------------------------------------------------
//  Libraries
// ----------------------------------------------------------------------------
#include <map>

// ----------------------------------------------------------------------------
//  nTiled headers
// ----------------------------------------------------------------------------
#include "state\State.h"

#include "ShaderDefinitions.h"
#include "forward-shading\ForwardShader.h"
#include "deferred-shading\DeferredShader.h"


namespace nTiled {
namespace pipeline {

/*!
 ShaderFactory adds both ForwardShaders and Deferred Shaders to the
 State it is initialised with.

 It is responsbile for constructing the shaders and adding it to
 the state.
 It provides the methods for retrieving Shaders from the state, as
 well as adding them when Shaders are not found.
 */
class ShaderFactory {
public:
  // constructor 
  /*!
   Construct a new ShaderFactory with the specified State reference

   Args:
        state (nTiled_State::State&): the state which holds the shaders
                                      shaders this ShaderFactory
                                      interacts with.

   Returns:
       A new ShaderFactory linked to the specified state.
   */
  ShaderFactory(state::State& state);

  /*!
   Get a ForwardShader of shader_id from this ShaderFactory

   Retrieves a pointer to the specified ForwardShaderBatch that
   corresponds with the specified shader_id.

   Args:
       shader_id (ForwardShaderId): ShaderId of the retrieved
                                    ForwardShader pointer.

   Returns:
       The pointer to the ForwardShader corresponding with the
       ForwardShaderId. This shader is linked to the state with
       which this ShaderFactory was initialised.
   */
  ForwardShader* getForwardShader(ForwardShaderId shader_id);

  /*!
   Get a DeferredShader of shader_id from this ShaderFactory

   Retrieves a pointer to the specified DeferredShaderBatch that
   corresponds with the specified shader_id.

   Args:
       shader_id (DeferredShaderId): ShaderId of the retrieved
                                     DeferredShader pointer.

   Returns:
       The pointer to the DeferredShader corresponding with the
       DeferredShaderId. This shader is linked to the state with
       which this ShaderFactory was initialised.
   */
  DeferredShader* getDeferredShader(DeferredShaderId shader_id);

private:
  /*!
   Add and return the pointer of the specified new ForwardShader

   Add the ForwardShader specified with shader_id to the state
   that was used to create this ShaderFactory
   and return the corresponding pointer.
   The shader itself is stored in the state.

   Args:
       shader_id (ForwardShaderId): ShaderId of the retrieved
                                    ForwardShader pointer.

   Returns:
       The pointer to the ForwardShader corresponding with the
       ForwardShaderId. This shader is linked to the state with
       which this ShaderFactory was initialised.
   */
  ForwardShader* addForwardShader(ForwardShaderId shader_id);

  /*!
   Add and return the pointer of the specified new DeferredShader

   Add the DeferredShader specified with shader_id to the state
   that was used to create this ShaderFactory
   and return the corresponding pointer.
   The shader itself is stored in the state.

   Args:
       shader_id (DeferredShaderId): ShaderId of the retrieved
                                     DeferredShader pointer.

   Returns:
       The pointer to the DeferredShader corresponding with the
       DeferredShaderId. This shader is linked to the state with
       which this ShaderFactory was initialised.
   */
  DeferredShader* addDeferredShader(DeferredShaderId shader_id);

  /*! State with which this ShaderFactory was initialised. */
  state::State& state;
};

} // pipeline
} // nTiled
