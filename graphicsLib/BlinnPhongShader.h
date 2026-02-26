//
// Created by Evan on 2/15/2026.
//

#ifndef CS4212STARTERCODE_BLINNPHONGSHADER_H
#define CS4212STARTERCODE_BLINNPHONGSHADER_
#include "Shader.h"
#include "Scene.h"

class BlinnPhongShader : public Shader {
private:
  Scene& scene;

public:
  BlinnPhongShader(Scene& scene) : scene(scene) {};

  color rayColor(const hitStructure &h, int depth);
};


#endif// CS4212STARTERCODE_BLINNPHONGSHADER_H
