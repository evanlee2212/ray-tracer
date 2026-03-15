//
// Created by Evan on 2/11/2026.
//

#pragma once

#include "Framebuffer.h"

struct hitStructure;

class Shader {
protected:
  vec3 baseColor;

public:
  virtual ~Shader() = default;
  virtual color rayColor(const hitStructure &h, int depth ) = 0;
  virtual void setColor(vec3 c_norm) { baseColor = c_norm * 255; };
  virtual vec3 getColor() { return baseColor; };
};

