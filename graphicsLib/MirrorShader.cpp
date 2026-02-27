//
// Created by Evan on 2/22/2026.
//

#include "MirrorShader.h"
#include "Scene.h"

color MirrorShader::rayColor(const hitStructure &h, int depth) {
  if (depth <= 0) {
    return baseColor;
  }

  vec3 reflection = reflect(h.r.direction(), h.normal);

  const double epsilon = 0.001;
  Ray reflectedRay = Ray(h.point + h.normal * epsilon, reflection);

  color result = scene.computeRaycolor(reflectedRay, 0.001f, FLT_MAX, depth-1);

  return result;
}
