//
// Created by Evan on 2/22/2026.
//

#ifndef CS4212STARTERCODE_MIRRORSHADER_H
#define CS4212STARTERCODE_MIRRORSHADER_H
#include "Scene.h"
#include "Shader.h"


class MirrorShader : public Shader  {
private:
  Scene& scene;

  public:
  MirrorShader(Scene& scene) : scene(scene) {};
  color rayColor(const hitStructure &h, int depth );
};


#endif// CS4212STARTERCODE_MIRRORSHADER_H
