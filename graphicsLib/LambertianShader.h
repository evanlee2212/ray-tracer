//
// Created by Evan on 2/11/2026.
//

#ifndef CS4212STARTERCODE_LAMBERTIANSHADER_H
#define CS4212STARTERCODE_LAMBERTIANSHADER_H
#include "Shader.h"
#include "Scene.h"

class LambertianShader : public Shader {
private:
  Scene& scene;

public:
  LambertianShader(Scene& scene) : scene(scene) {};

  color rayColor(const hitStructure &h, int depth);

};

#endif// CS4212STARTERCODE_LAMBERTIANSHADER_H
