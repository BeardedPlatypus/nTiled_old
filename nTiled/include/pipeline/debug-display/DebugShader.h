#pragma once

namespace nTiled {
namespace pipeline {
class DebugShader {
 public:
  virtual ~DebugShader() {}

  virtual void render() = 0;
};

} // pipeline
} // nTiled