//
// Created by Evan on 2/19/2026.
//
#include "LambertianShader.h"

#include "Scene.h"

color LambertianShader::rayColor(const hitStructure &h, int depth) {
  point3 lightPos(0, -50, 0);

  vec3 dirToLight = unit_vector(lightPos - h.point);

  double ndot1 = std::max(0.0, dot(h.normal, dirToLight));

  color result = h.color * ndot1;

  return result;
}