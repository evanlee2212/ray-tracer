//
// Created by Evan on 2/11/2026.
//

#ifndef CS4212STARTERCODE_LAMBERTIANSHADER_H
#define CS4212STARTERCODE_LAMBERTIANSHADER_H
#include "Shader.h"

class LambertianShader : public Shader {

public:
  LambertianShader() {};

  color rayColor(const hitStructure &h, int depth);

};

#endif// CS4212STARTERCODE_LAMBERTIANSHADER_H
