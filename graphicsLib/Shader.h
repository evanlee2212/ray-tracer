//
// Created by Evan on 2/11/2026.
//

#pragma once

#include "Framebuffer.h"

struct hitStructure;

class Shader {
protected:
  vec3 baseColor = vec3(50, 50, 50);

public:
  virtual ~Shader() = default;
  virtual color rayColor(const hitStructure &h, int depth ) = 0;
};

